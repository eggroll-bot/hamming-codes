SOURCEFILES_1 = hamming_encode.c
OBJECTFILES_1 = hamming_encode.o
OUTPUT_1 = hamming_encode

SOURCEFILES_2 = hamming_decode.c
OBJECTFILES_2 = hamming_decode.o
OUTPUT_2 = hamming_decode

SOURCEFILES_DEPENDENCIES_1_2 = hamming.c
OBJECTFILES_DEPENDENCIES_1_2 = hamming.o

CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Ofast
LDFLAGS = -flto -Ofast

.PHONY: all debug clean format

all: $(OUTPUT_1) $(OUTPUT_2)

$(OUTPUT_1): $(OBJECTFILES_1) $(OBJECTFILES_DEPENDENCIES_1_2)
	$(CC) $(LDFLAGS) -o $(OUTPUT_1) $(OBJECTFILES_1) $(OBJECTFILES_DEPENDENCIES_1_2)

$(OUTPUT_2): $(OBJECTFILES_2) $(OBJECTFILES_DEPENDENCIES_1_2)
	$(CC) $(LDFLAGS) -o $(OUTPUT_2) $(OBJECTFILES_2) $(OBJECTFILES_DEPENDENCIES_1_2)

$(OBJECTFILES_1): $(SOURCEFILES_1)
	$(CC) $(CFLAGS) -c $(SOURCEFILES_1)

$(OBJECTFILES_2): $(SOURCEFILES_2)
	$(CC) $(CFLAGS) -c $(SOURCEFILES_2)

$(OBJECTFILES_DEPENDENCIES_1_2): $(SOURCEFILES_DEPENDENCIES_1_2)
	$(CC) $(CFLAGS) -c $(SOURCEFILES_DEPENDENCIES_1_2)

debug: CFLAGS += -g -O0
debug: CFLAGS := $(filter-out -Ofast, $(CFLAGS))
debug: LDFLAGS += -O0
debug: LDFLAGS := $(filter-out -flto -Ofast, $(LDFLAGS))
debug: all

clean:
	rm -f $(OUTPUT_1) $(OUTPUT_2) $(OBJECTFILES_1) $(OBJECTFILES_2) $(OBJECTFILES_DEPENDENCIES_1_2)

format:
	clang-format -i -style=file *.[ch]
