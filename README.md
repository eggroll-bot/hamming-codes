# Hamming Codes

Contains two Hamming(8, 4) code encoder and decoder implementations.

The encoders and decoders will read from stdin and output to stdout by default, but flags can be specified to make it read from or output to specified files instead.

The encoder and decoder in the `lookup_table` folder uses a lookup table to encode and decode Hamming(8, 4) codes.

The encoder and decoder in the `matrix_multiplication` folder uses matrix multiplication with memoization to encode and decode Hamming(8, 4) codes. This encoder and decoder was used to generate the lookup tables for the lookup table encoder and decoder.

Adapted from a Computer Systems and C Programming course assignment.

## How to build

To build all of the programs, run `make`.

The Makefile has the following targets:

- all - builds the programs (default),
- debug - builds the programs with no optimizations and with debug info,
- clean - removes the built programs and object files created by the building process,
- format - formats all .c and .h files using a .clang-format file.

## How to run

To see the program usage text, navigate to the directory with the implementation you want to run and run `./hamming_encode -h` and `./hamming_decode -h` after building it.

For the encoder programs, use the `-h` flag to print the program usage and help, the `-i` flag with an argument to specify an input file, and the `-o` flag with an argument to specify an output file.

For the decoder programs, use the `-h` flag to print the program usage and help, the `-v` flag to print decoding statistics to stderr, the `-i` flag with an argument to specify an input file, and the `-o` flag with an argument to specify an output file.

By default, the encoder and decoder programs will use stdin for the input and stdout for the output. In error cases, stderr will be used. The decoder will output decoding statistics to stderr with the `-v` flag.

## Known issues

None.
