#include "hamming.h"

#include "bm.h"

#include <stdint.h>

// Description:
// Encodes a 4-bit message into a Hamming(8, 4) code.
//
// Parameters:
// BitMatrix *g - The generator matrix.
// uint8_t msg - The message to encode.
//
// Returns:
// uint8_t - The Hamming(8, 4) code.
uint8_t ham_encode( BitMatrix *g, uint8_t msg ) {
	// Array used to cache encode results.
	static uint8_t encode_lookup[ 16 ] = { 0 };
	uint8_t lower_nibble = msg & 0xF;
	uint8_t result = 0;

	if ( lower_nibble != 0 && encode_lookup[ lower_nibble ] == 0 ) { // Not in cache.
		BitMatrix *msg_matrix = bm_from_data( lower_nibble, 4 );
		BitMatrix *result_matrix = bm_multiply( msg_matrix, g );
		result = bm_to_data( result_matrix );
		encode_lookup[ lower_nibble ] = result; // Add to cache.
		bm_delete( &result_matrix );
		bm_delete( &msg_matrix );
	} else { // Found in cache.
		result = encode_lookup[ lower_nibble ];
	}

	return result;
}

// Description:
// Decodes a Hamming(8, 4) code to a 4-bit message.
//
// Parameters:
// BitMatrix *ht - The transpose of the parity checker matrix.
// uint8_t code - The Hamming(8, 4) code.
// uint8_t *msg - Where to put the decoded message. Will be unmodified upon failure.
//
// Returns:
// HAM_STATUS - Whether the hamming code could be successfully decoded.
HAM_STATUS ham_decode( BitMatrix *ht, uint8_t code, uint8_t *msg ) {
	// Array used to cache error syndrome calculations.
	static uint8_t error_syndrome_calculate_lookup[ 256 ] = { 0 };
	// Lookup table for corrections to make with a certain error syndrome.
	static const int8_t error_syndrome_corrections[] = { HAM_OK, 4, 5, HAM_ERR, 6, HAM_ERR, HAM_ERR, 3, 7, HAM_ERR, HAM_ERR, 2, HAM_ERR, 1, 0, HAM_ERR };
	uint8_t error_syndrome = 0;

	if ( error_syndrome_calculate_lookup[ code ] == 0 ) { // Not in cache.
		BitMatrix *code_matrix = bm_from_data( code, 8 );
		BitMatrix *error_syndrome_matrix = bm_multiply( code_matrix, ht );
		error_syndrome = bm_to_data( error_syndrome_matrix );
		// Bias the error syndrome in the cache, so 0 is reserved for "no value found".
		error_syndrome_calculate_lookup[ code ] = error_syndrome + 1; // Add to cache.
		bm_delete( &error_syndrome_matrix );
		bm_delete( &code_matrix );
	} else { // Found in cache.
		// Reverse the bias in the cache for the error syndrome.
		error_syndrome = error_syndrome_calculate_lookup[ code ] - 1;
	}

	int8_t correct = error_syndrome_corrections[ error_syndrome ];

	if ( correct < 0 ) { // HAM_ERR or HAM_OK.
		if ( correct == HAM_OK ) {
			*msg = ( code & 0xF );
		}

		return correct;
	} else { // Correctable error.
		if ( correct <= 3 ) {
			uint8_t corrected = code ^ ( 1 << correct ); // Flip the bit to be corrected.
			*msg = ( corrected & 0xF );
		} else {
			*msg = ( code & 0xF );
		}

		return HAM_CORRECT;
	}
}
