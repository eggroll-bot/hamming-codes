#include "bv.h"

#include <stdint.h>
#include <stdlib.h>

// Description:
// A struct for the bit vector ADT.
//
// Members:
// uint32_t length - The number of bits in the bit vector.
// uint8_t *vector - The array of bytes that will hold the bits.
struct BitVector {
	uint32_t length;
	uint8_t *vector;
};

// Description:
// Creates a bit vector.
//
// Parameters:
// uint32_t length - The length of the bit vector in bits.
//
// Returns:
// BitVector * - The created bit vector.
BitVector *bv_create( uint32_t length ) {
	BitVector *v = malloc( sizeof( BitVector ) );

	if ( v ) { // Make sure v was allocated successfully.
		v->length = length;
		v->vector = calloc( 1, length % 8 == 0 ? length / 8 : length / 8 + 1 );

		if ( !v->vector ) {
			free( v );
			v = NULL;
		}
	}

	return v;
}

// Description:
// Deletes a bit vector.
//
// Parameters:
// BitVector **v - A pointer to a pointer to a bit vector.
//
// Returns:
// Nothing.
void bv_delete( BitVector **v ) {
	if ( *v && ( *v )->vector ) {
		free( ( *v )->vector );
		free( *v );
		*v = NULL;
	}
}

// Description:
// Gets a bit in a bit vector.
//
// Parameters:
// BitVector *v - A pointer to a bit vector.
// uint32_t i - The bit position to get.
//
// Returns:
// uint8_t - The bit in the bit position specified.
uint8_t bv_get_bit( BitVector *v, uint32_t i ) {
	return 1 & ( v->vector[ i / 8 ] >> ( i % 8 ) );
}

// Description:
// Sets a bit in a bit vector.
//
// Parameters:
// BitVector *v - A pointer to a bit vector.
// uint32_t i - The bit position to set.
//
// Returns:
// Nothing.
void bv_set_bit( BitVector *v, uint32_t i ) {
	v->vector[ i / 8 ] |= ( 1 << ( i % 8 ) );
}

// Description:
// Gets a byte in a bit vector.
//
// Parameters:
// BitVector *v - A pointer to a bit vector.
// uint32_t i - The byte position to get.
//
// Returns:
// uint8_t - The byte in the byte position specified.
uint8_t bv_get_byte( BitVector *v, uint32_t i ) {
	return v->vector[ i ];
}

// Description:
// Sets a byte in a bit vector.
//
// Parameters:
// BitVector *v - A pointer to a bit vector.
// uint32_t i - The byte position to set.
// uint8_t byte - The byte to set the contents of the byte position to.
//
// Returns:
// Nothing.
void bv_set_byte( BitVector *v, uint32_t i, uint8_t byte ) {
	v->vector[ i ] = byte;
}
