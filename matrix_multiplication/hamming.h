#ifndef __HAMMING_H__
#define __HAMMING_H__

#include "bm.h"

#include <stdint.h>

typedef enum HAM_STATUS {
	HAM_OK = -3, // No errors detected.
	HAM_ERR = -2, // Uncorrectable.
	HAM_CORRECT = -1, // Detected error and corrected.
} HAM_STATUS;

uint8_t ham_encode( BitMatrix *g, uint8_t msg );

HAM_STATUS ham_decode( BitMatrix *ht, uint8_t code, uint8_t *msg );

#endif
