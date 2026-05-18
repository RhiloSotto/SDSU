CXX      = mpicxx
NVCC     = nvcc
TARGET   = simpson_mpi_cuda
SRC      = simpson_mpi_cuda.cu

N ?= 100000000
OUT ?= results.txt

NVCCFLAGS = -O3 -std=c++11 -ccbin $(CXX)

all: $(TARGET)

$(TARGET): $(SRC)
	$(NVCC) $(NVCCFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	@rm -f $(OUT)
	@echo "Running tests with N=$(N)"
	@for M in 2 4 8; do \
		for T in 1 2 4 8 16 32 64 128 256 512 1024 2048; do \
			echo "M=$$M T=$$T"; \
			mpirun -mca btl "^openib" -np $$M ./$(TARGET) -n $(N) -t $$T >> $(OUT); \
		done; \
	done
	@echo "Results saved to $(OUT)"

clean:
	rm -f $(TARGET) *.o $(OUT) runtime_surface.png