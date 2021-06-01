#include "bm.h"

#include "bv.h"

#include <stdint.h>
#include <stdlib.h>

// Description:
// A struct for the bit matrix ADT.
//
// Members:
// uint32_t rows - The number of rows in the bit matrix.
// uint32_t cols - The number of columns in the bit matrix.
// BitVector *vector - The bit vector that backs the bit matrix.
struct BitMatrix {
	uint32_t rows;
	uint32_t cols;
	BitVector *vector;
};

// Description:
// Creates a bit matrix.
//
// Parameters:
// uint32_t rows - The number of rows in the bit matrix.
// uint32_t cols - The number of columns in the bit matrix.
//
// Returns:
// BitMatrix * - The created bit matrix.
BitMatrix *bm_create( uint32_t rows, uint32_t cols ) {
	BitMatrix *m = malloc( sizeof( BitMatrix ) );

	if ( m ) { // Make sure m was allocated successfully.
		m->rows = rows;
		m->cols = cols;
		m->vector = bv_create( rows * cols );

		if ( !m->vector ) {
			free( m );
			m = NULL;
		}
	}

	return m;
}

// Description:
// Deletes a bit matrix.
//
// Parameters:
// BitMatrix **m - A pointer to a pointer to a bit matrix.
//
// Returns:
// Nothing.
void bm_delete( BitMatrix **m ) {
	if ( *m && ( *m )->vector ) {
		bv_delete( &( *m )->vector );
		free( *m );
		*m = NULL;
	}
}

// Description:
// Gets a bit in a bit matrix.
//
// Parameters:
// BitMatrix *m - A pointer to a bit matrix.
// uint32_t row - The row of the bit matrix to get.
// uint32_t col - The column of the bit matrix to get.
//
// Returns:
// uint8_t - The bit retrieved.
uint8_t bm_get_bit( BitMatrix *m, uint32_t row, uint32_t col ) {
	return bv_get_bit( m->vector, row * m->cols + col );
}

// Description:
// Sets a bit in a bit matrix.
//
// Parameters:
// BitMatrix *m - A pointer to a bit matrix.
// uint32_t row - The row of the bit matrix to set.
// uint32_t col - The column of the bit matrix to set.
//
// Returns:
// Nothing.
void bm_set_bit( BitMatrix *m, uint32_t row, uint32_t col ) {
	bv_set_bit( m->vector, row * m->cols + col );
}

// Description:
// Turns up to a byte of data into a 1 x length bit matrix.
//
// Parameters:
// uint8_t byte - The data to turn into a bit matrix.
// uint32_t length - The length of the data.
//
// Returns:
// BitMatrix * - A 1 x length bit matrix containing the data.
BitMatrix *bm_from_data( uint8_t byte, uint32_t length ) {
	BitMatrix *m = bm_create( 1, length );
	bv_set_byte( m->vector, 0, byte );

	return m;
}

// Description:
// Returns the first byte of a bit matrix.
//
// Parameters:
// BitMatrix *m - A pointer to a bit matrix.
//
// Returns:
// uint8_t - The first byte of the bit matrix.
uint8_t bm_to_data( BitMatrix *m ) {
	return bv_get_byte( m->vector, 0 );
}

// Description:
// Multiplies two bit matrices together.
//
// Parameters:
// BitMatrix *a - A pointer to the first bit matrix.
// BitMatrix *b - A pointer to the second bit matrix.
//
// Returns:
// BitMatrix * - The result of the matrices being multiplied.
BitMatrix *bm_multiply( BitMatrix *a, BitMatrix *b ) {
	BitMatrix *m = bm_create( a->rows, b->cols );

	for ( uint32_t row = 0; row < m->rows; row++ ) {
		for ( uint32_t col = 0; col < m->cols; col++ ) {
			uint8_t result = 0;

			for ( uint32_t a_col = 0; a_col < a->cols; a_col++ ) {
				// ^ = add % 2, & = multiply % 2.
				result ^= ( bm_get_bit( a, row, a_col ) & bm_get_bit( b, a_col, col ) );
			}

			if ( result ) {
				bm_set_bit( m, row, col );
			}
		}
	}

	return m;
}
