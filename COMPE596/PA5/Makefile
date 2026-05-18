all: mmult

mmult: mmult.cu
	nvcc mmult.cu -o mmult

N = 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28
OUTPUT_FILE = data.txt

run: mmult
	@rm -f $(OUTPUT_FILE)
	@for N in $(N); do \
		./mmult $$N >> $(OUTPUT_FILE); \
	done \
