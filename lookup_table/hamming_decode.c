#include "hamming.h"

#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define OPTIONS "hvi:o:" // Valid options for the program.

static FILE *input_file = NULL;
static FILE *output_file = NULL;

// Description:
// Prints the help message to stderr.
//
// Parameters:
// char *program_path - The path to the program.
//
// Returns:
// Nothing.
static void print_help( char *program_path ) {
	fprintf( stderr,
	    "SYNOPSIS\n   A Hamming(8, 4) code decoder using a lookup table.\n\nUSAGE\n   %s [-hv] [-i infile] [-o outfile]\n\nOPTIONS\n   -h             Program usage and help.\n   -v             Print "
	    "decoding statistics to stderr.\n   -i infile      Input file to decode.\n   -o outfile     File to output decoded data to.\n",
	    program_path );
}

// Description:
// Cleans up memory used by the program if it's been allocated.
//
// Parameters:
// Nothing.
//
// Returns:
// Nothing.
static void cleanup_memory( ) {
	if ( output_file ) {
		fclose( output_file );
		output_file = NULL;
	}

	if ( input_file ) {
		fclose( input_file );
		input_file = NULL;
	}
}

// Description:
// Processes the file names inputted by the user.
//
// Parameters:
// char *input_file_name - The input file name given by the user.
// char *output_file_name - The output file name given by the user.
//
// Returns:
// bool - Whether processing was successful.
static bool process_input_output_files( char *input_file_name, char *output_file_name ) {
	if ( input_file_name && !( input_file = fopen( input_file_name, "rb" ) ) ) {
		fprintf( stderr, "Error: failed to open infile.\n" );

		return false;
	}

	if ( output_file_name && !( output_file = fopen( output_file_name, "wb" ) ) ) {
		fprintf( stderr, "Error: failed to open outfile.\n" );
		cleanup_memory( );

		return false;
	}

	if ( input_file_name && output_file_name ) {
		struct stat input_file_stats;
		fstat( fileno( input_file ), &input_file_stats ); // Get input file metadata.
		fchmod( fileno( output_file ), input_file_stats.st_mode ); // Set permissions of output file.
	}

	return true;
}

// Description:
// Decodes input file two characters at a time and outputs the decoded data to the output file.
//
// Parameters:
// uint64_t *total_bytes_processed - A pointer to the counter for total number of bytes processed.
// uint64_t *uncorrectable_errors - A pointer to the counter for uncorrectable errors.
// uint64_t *corrected_errors - A pointer to the counter for corrected errors.
//
// Returns:
// bool - Whether the data could be read, decoded, and written to the output file.
static bool decode_and_write_to_file( uint64_t *total_bytes_processed, uint64_t *uncorrectable_errors, uint64_t *corrected_errors ) {
	uint8_t lower_nibble = 0;
	uint8_t upper_nibble = 0;
	uint32_t last_scan = 1;

	while ( last_scan == 1 ) {
		int first_input_get_result = fgetc( input_file );
		int second_input_get_result = fgetc( input_file );

		if ( ferror( input_file ) ) {
			fprintf( stderr, "Error: failed to read from input file.\n" );
			cleanup_memory( );

			return false;
		}

		if ( first_input_get_result == EOF ) {
			last_scan = 0;
		} else if ( second_input_get_result == EOF ) {
			last_scan = 0;
			total_bytes_processed++;
		} else {
			total_bytes_processed += 2;
			uint8_t first_byte = first_input_get_result;
			uint8_t second_byte = second_input_get_result;
			// Decode lower nibble.
			HAM_STATUS lower_nibble_status = ham_decode( first_byte, &lower_nibble );

			if ( lower_nibble_status == HAM_ERR ) {
				uncorrectable_errors++;
			} else if ( lower_nibble_status == HAM_CORRECT ) {
				corrected_errors++;
			}

			// Decode upper nibble.
			HAM_STATUS upper_nibble_status = ham_decode( second_byte, &upper_nibble );

			if ( upper_nibble_status == HAM_ERR ) {
				uncorrectable_errors++;
			} else if ( upper_nibble_status == HAM_CORRECT ) {
				corrected_errors++;
			}

			// Output 0 upon failure.
			if ( lower_nibble_status == HAM_ERR || upper_nibble_status == HAM_ERR ) {
				lower_nibble = 0;
				upper_nibble = 0;
			}

			uint8_t reconstructed_byte = ( upper_nibble << 4 ) | lower_nibble;
			int write_byte_result = fputc( reconstructed_byte, output_file );

			if ( write_byte_result == EOF ) {
				fprintf( stderr, "Error: failed to write to output file.\n" );
				cleanup_memory( );

				return false;
			}
		}
	}

	return true;
}

// Description:
// The entry point of the program.
//
// Parameters:
// int argc - The argument count.
// char **argv - An array of argument strings.
//
// Returns:
// int - The exit status of the program (0 = success, otherwise error).
int main( int argc, char **argv ) {
	int opt = 0;
	bool verbose = false;
	char *input_file_name = NULL;
	char *output_file_name = NULL;

	while ( ( opt = getopt( argc, argv, OPTIONS ) ) != -1 ) { // Process each option specified.
		switch ( opt ) {
		case 'h': print_help( *argv ); return 0; // Help.
		case 'v': verbose = true; break; // Verbose.
		case 'i': input_file_name = optarg; break; // Input file.
		case 'o': output_file_name = optarg; break; // Output file.
		default: print_help( *argv ); return 1; // Invalid flag.
		}
	}

	input_file = stdin;
	output_file = stdout;

	if ( !process_input_output_files( input_file_name, output_file_name ) ) {
		return 1;
	}

	uint64_t total_bytes_processed = 0;
	uint64_t uncorrectable_errors = 0;
	uint64_t corrected_errors = 0;

	if ( !decode_and_write_to_file( &total_bytes_processed, &uncorrectable_errors, &corrected_errors ) ) {
		return 1;
	}

	if ( verbose ) {
		double error_rate = ( double ) uncorrectable_errors / total_bytes_processed;
		fprintf( stderr, "Total bytes processed: %" PRIu64 "\n", total_bytes_processed );
		fprintf( stderr, "Uncorrectable errors: %" PRIu64 "\n", uncorrectable_errors );
		fprintf( stderr, "Corrected errors: %" PRIu64 "\n", corrected_errors );
		fprintf( stderr, "Error rate: %f\n", error_rate );
	}

	cleanup_memory( );

	return 0;
}
