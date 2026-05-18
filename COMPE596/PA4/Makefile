all: reduction

reduction: reduction.cu
	nvcc reduction.cu -o reduction

N = 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28
OUTPUT_FILE = data.txt

run: reduction
	@rm -f $(OUTPUT_FILE)
	@for N in $(N); do \
		./reduction $$N >> $(OUTPUT_FILE); \
	done \
