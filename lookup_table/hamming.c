#include "hamming.h"

#include <stdint.h>

// Description:
// Encodes a 4-bit message into a Hamming(8, 4) code.
//
// Parameters:
// uint8_t msg - The message to encode.
//
// Returns:
// uint8_t - The Hamming(8, 4) code.
uint8_t ham_encode( uint8_t msg ) {
	static const uint8_t encode_lookup[ 16 ] = { 0, 225, 210, 51, 180, 85, 102, 135, 120, 153, 170, 75, 204, 45, 30, 255 };

	return encode_lookup[ msg & 0xF ];
}

// Description:
// Decodes a Hamming(8, 4) code to a 4-bit message.
//
// Parameters:
// uint8_t code - The Hamming(8, 4) code.
// uint8_t *msg - Where to put the decoded message. Will be unmodified upon failure.
//
// Returns:
// HAM_STATUS - Whether the hamming code could be successfully decoded.
HAM_STATUS ham_decode( uint8_t code, uint8_t *msg ) {
	static const int8_t error_syndrome_corrections[ 256 ] = { HAM_OK, 0, 1, HAM_ERR, 2, HAM_ERR, HAM_ERR, 7, 3, HAM_ERR, HAM_ERR, 6, HAM_ERR, 5, 4, HAM_ERR, 4, HAM_ERR, HAM_ERR, 5, HAM_ERR, 6, 3,
		HAM_ERR, HAM_ERR, 7, 2, HAM_ERR, 1, HAM_ERR, HAM_OK, 0, 5, HAM_ERR, HAM_ERR, 4, HAM_ERR, 3, 6, HAM_ERR, HAM_ERR, 2, 7, HAM_ERR, 0, HAM_OK, HAM_ERR, 1, HAM_ERR, 1, 0, HAM_OK, 7, HAM_ERR,
		HAM_ERR, 2, 6, HAM_ERR, HAM_ERR, 3, HAM_ERR, 4, 5, HAM_ERR, 6, HAM_ERR, HAM_ERR, 3, HAM_ERR, 4, 5, HAM_ERR, HAM_ERR, 1, 0, HAM_OK, 7, HAM_ERR, HAM_ERR, 2, HAM_ERR, 2, 7, HAM_ERR, 0, HAM_OK,
		HAM_ERR, 1, 5, HAM_ERR, HAM_ERR, 4, HAM_ERR, 3, 6, HAM_ERR, HAM_ERR, 7, 2, HAM_ERR, 1, HAM_ERR, HAM_OK, 0, 4, HAM_ERR, HAM_ERR, 5, HAM_ERR, 6, 3, HAM_ERR, 3, HAM_ERR, HAM_ERR, 6, HAM_ERR, 5,
		4, HAM_ERR, HAM_OK, 0, 1, HAM_ERR, 2, HAM_ERR, HAM_ERR, 7, 7, HAM_ERR, HAM_ERR, 2, HAM_ERR, 1, 0, HAM_OK, HAM_ERR, 4, 5, HAM_ERR, 6, HAM_ERR, HAM_ERR, 3, HAM_ERR, 3, 6, HAM_ERR, 5, HAM_ERR,
		HAM_ERR, 4, 0, HAM_OK, HAM_ERR, 1, HAM_ERR, 2, 7, HAM_ERR, HAM_ERR, 6, 3, HAM_ERR, 4, HAM_ERR, HAM_ERR, 5, 1, HAM_ERR, HAM_OK, 0, HAM_ERR, 7, 2, HAM_ERR, 2, HAM_ERR, HAM_ERR, 7, HAM_OK, 0, 1,
		HAM_ERR, HAM_ERR, 5, 4, HAM_ERR, 3, HAM_ERR, HAM_ERR, 6, HAM_ERR, 5, 4, HAM_ERR, 3, HAM_ERR, HAM_ERR, 6, 2, HAM_ERR, HAM_ERR, 7, HAM_OK, 0, 1, HAM_ERR, 1, HAM_ERR, HAM_OK, 0, HAM_ERR, 7, 2,
		HAM_ERR, HAM_ERR, 6, 3, HAM_ERR, 4, HAM_ERR, HAM_ERR, 5, 0, HAM_OK, HAM_ERR, 1, HAM_ERR, 2, 7, HAM_ERR, HAM_ERR, 3, 6, HAM_ERR, 5, HAM_ERR, HAM_ERR, 4, HAM_ERR, 4, 5, HAM_ERR, 6, HAM_ERR,
		HAM_ERR, 3, 7, HAM_ERR, HAM_ERR, 2, HAM_ERR, 1, 0, HAM_OK };

	int8_t correct = error_syndrome_corrections[ code ];

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
