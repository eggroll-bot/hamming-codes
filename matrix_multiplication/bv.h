#ifndef __BV_H__
#define __BV_H__

#include <stdint.h>

typedef struct BitVector BitVector;

BitVector *bv_create( uint32_t length );

void bv_delete( BitVector **v );

uint8_t bv_get_bit( BitVector *v, uint32_t i );

void bv_set_bit( BitVector *v, uint32_t i );

uint8_t bv_get_byte( BitVector *v, uint32_t i );

void bv_set_byte( BitVector *v, uint32_t i, uint8_t byte );

#endif
