CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Ofast
LDFLAGS = -flto -Ofast

.PHONY: all

all: lookup_table/hamming_encode lookup_table/hamming_decode matrix_multiplication/hamming_encode matrix_multiplication/hamming_decode

lookup_table/hamming_encode lookup_table/hamming_decode:
	$(MAKE) -C lookup_table

matrix_multiplication/hamming_encode matrix_multiplication/hamming_decode:
	$(MAKE) -C matrix_multiplication
