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
	return msg;
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
	return code + *msg - code - *msg + HAM_OK;
}
