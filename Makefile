.PHONY: all debug clean format

all: lookup_table matrix_multiplication
	$(MAKE) -C lookup_table all
	$(MAKE) -C matrix_multiplication all

debug: lookup_table matrix_multiplication
	$(MAKE) -C lookup_table debug
	$(MAKE) -C matrix_multiplication debug

clean: lookup_table matrix_multiplication
	$(MAKE) -C lookup_table clean
	$(MAKE) -C matrix_multiplication clean

format: lookup_table matrix_multiplication
	$(MAKE) -C lookup_table format
	$(MAKE) -C matrix_multiplication format
