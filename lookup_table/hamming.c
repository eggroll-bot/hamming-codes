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
	static const uint8_t error_syndrome_calculate_lookup[ 256 ] = { 0, 14, 13, 3, 11, 5, 6, 8, 7, 9, 10, 4, 12, 2, 1, 15, 1, 15, 12, 2, 10, 4, 7, 9, 6, 8, 11, 5, 13, 3, 0, 14, 2, 12, 15, 1, 9, 7, 4,
		10, 5, 11, 8, 6, 14, 0, 3, 13, 3, 13, 14, 0, 8, 6, 5, 11, 4, 10, 9, 7, 15, 1, 2, 12, 4, 10, 9, 7, 15, 1, 2, 12, 3, 13, 14, 0, 8, 6, 5, 11, 5, 11, 8, 6, 14, 0, 3, 13, 2, 12, 15, 1, 9, 7, 4, 10,
		6, 8, 11, 5, 13, 3, 0, 14, 1, 15, 12, 2, 10, 4, 7, 9, 7, 9, 10, 4, 12, 2, 1, 15, 0, 14, 13, 3, 11, 5, 6, 8, 8, 6, 5, 11, 3, 13, 14, 0, 15, 1, 2, 12, 4, 10, 9, 7, 9, 7, 4, 10, 2, 12, 15, 1, 14,
		0, 3, 13, 5, 11, 8, 6, 10, 4, 7, 9, 1, 15, 12, 2, 13, 3, 0, 14, 6, 8, 11, 5, 11, 5, 6, 8, 0, 14, 13, 3, 12, 2, 1, 15, 7, 9, 10, 4, 12, 2, 1, 15, 7, 9, 10, 4, 11, 5, 6, 8, 0, 14, 13, 3, 13, 3,
		0, 14, 6, 8, 11, 5, 10, 4, 7, 9, 1, 15, 12, 2, 14, 0, 3, 13, 5, 11, 8, 6, 9, 7, 4, 10, 2, 12, 15, 1, 15, 1, 2, 12, 4, 10, 9, 7, 8, 6, 5, 11, 3, 13, 14, 0 };
	static const int8_t error_syndrome_corrections[ 16 ] = { HAM_OK, 4, 5, HAM_ERR, 6, HAM_ERR, HAM_ERR, 3, 7, HAM_ERR, HAM_ERR, 2, HAM_ERR, 1, 0, HAM_ERR };

	int8_t correct = error_syndrome_corrections[ error_syndrome_calculate_lookup[ code ] ];

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
