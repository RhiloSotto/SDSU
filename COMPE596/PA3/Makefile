all: quad

quad: quad.o
	gcc -fopenmp quad.c -o quad -lm

THREADS = 1 2 4 8 16 32 64 128 256 512 1024
OUTPUT_FILE = data.txt

run: quad
	@for thread in $(THREADS); do \
		./quad $$threads > $(OUTPUT_FILE); \
	done \
