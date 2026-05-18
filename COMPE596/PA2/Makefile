all: bll_insert

bll_insert: bll_insert.o
	gcc -fopenmp bll_insert.c -o bll_insert -lm

THREADS = 1 2 4 8 16 32 64 128 256 512 1024
N = 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18
OUTPUT_FILE = data.txt

run: bll_insert
	@for thread in $(THREADS); do \
		for node in $(N); do \
			./bll_insert.c $$threads $$node > $(OUTPUT_FILE); \
		done \
	done \