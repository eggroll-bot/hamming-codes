#include "bm.h"
#include "hamming.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define OPTIONS "hi:o:" // Valid options for the program.

static FILE *input_file = NULL;
static FILE *output_file = NULL;
static BitMatrix *generator_matrix = NULL;

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
	    "SYNOPSIS\n   A Hamming(8, 4) code generator using matrix multiplication with memoization.\n\nUSAGE\n   %s [-h] [-i infile] [-o outfile]\n\nOPTIONS\n   -h             Program usage and "
	    "help.\n   -i infile      Input file to encode.\n   -o outfile     File to output encoded data to.\n",
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
	if ( generator_matrix ) {
		bm_delete( &generator_matrix );
	}

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
// Initializes the generator matrix used for generating Hamming codes.
//
// Parameters:
// Nothing.
//
// Returns:
// Nothing.
static void initialize_generator_matrix( ) {
	static const uint8_t generator_matrix_array[ 4 ][ 8 ] = { { 1, 0, 0, 0, 0, 1, 1, 1 }, { 0, 1, 0, 0, 1, 0, 1, 1 }, { 0, 0, 1, 0, 1, 1, 0, 1 }, { 0, 0, 0, 1, 1, 1, 1, 0 } };
	BitMatrix *m = bm_create( 4, 8 );

	for ( uint8_t row = 0; row < 4; row++ ) {
		for ( uint8_t col = 0; col < 8; col++ ) {
			if ( generator_matrix_array[ row ][ col ] ) {
				bm_set_bit( m, row, col );
			}
		}
	}

	generator_matrix = m;
}

// Description:
// Encodes input file character-by-character and outputs the code to the output file.
//
// Parameters:
// Nothing.
//
// Returns:
// bool - Whether the data could be read, encoded, and written to the output file.
static bool encode_and_write_to_file( ) {
	uint32_t last_scan = 1;

	while ( last_scan == 1 ) {
		int input_get_result = fgetc( input_file );

		if ( ferror( input_file ) ) {
			fprintf( stderr, "Error: failed to read from input file.\n" );
			cleanup_memory( );

			return false;
		}

		if ( input_get_result != EOF ) {
			uint8_t byte = input_get_result;
			// Encode lower and upper nibble, then output the encoded result.
			int lower_nibble_result = fputc( ham_encode( generator_matrix, byte & 0xF ), output_file );
			int upper_nibble_result = fputc( ham_encode( generator_matrix, byte >> 4 ), output_file );

			if ( lower_nibble_result == EOF || upper_nibble_result == EOF ) {
				fprintf( stderr, "Error: failed to write to output file.\n" );
				cleanup_memory( );

				return false;
			}
		} else {
			last_scan = 0;
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
	char *input_file_name = NULL;
	char *output_file_name = NULL;

	while ( ( opt = getopt( argc, argv, OPTIONS ) ) != -1 ) { // Process each option specified.
		switch ( opt ) {
		case 'h': print_help( *argv ); return 0; // Help.
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

	initialize_generator_matrix( );

	if ( !encode_and_write_to_file( ) ) {
		return 1;
	}

	cleanup_memory( );

	return 0;
}
