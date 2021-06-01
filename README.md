# Hamming Codes

A Hamming code encoder and decoder implementation. The encoder and decoder will read from stdin and output to stdout by default, but flags can be specified to make it read from or output to specified files instead.

Adapted from a Computer Systems and C Programming course assignment.

## How to build

To build all of the programs, run `make`.

The Makefile has the following targets:

- all - builds the program (default),
- debug - builds the program with no optimizations and with debug info,
- clean - removes the built program and object files created by the building process,
- format - formats all .c and .h files using a .clang-format file.

## How to run

To see the program usage text, run `./hamming_encode -h` and `./hamming_decode -h` after building it.

For the encoder program, use the `-h` flag to print the program usage and help, the `-i` flag with an argument to specify an input file, and the `-o` flag with an argument to specify an output file.

For the decoder program, use the `-h` flag to print the program usage and help, the `-v` flag to print decoding statistics to stderr, the `-i` flag with an argument to specify an input file, and the `-o` flag with an argument to specify an output file.

By default, the encoder and decoder programs will use stdin for the input and stdout for the output. In error cases, stderr will be used. The decoder will output decoding statistics to stderr with the `-v` flag.

## Known issues

None.
