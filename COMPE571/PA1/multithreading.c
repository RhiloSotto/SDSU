// multithreading.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

// global variables
// all threads need to know and are constant
long N = 0;
int NUM_THREADS = 0;

void *partialSum(void *arg) {
	// (partial) summation
	long *partial_sum = malloc(sizeof(long));
	*partial_sum = 0;
	for (long i = *(int*)arg; i < N; i += NUM_THREADS) {
		*partial_sum += i;
	}
	pthread_exit(partial_sum);
}

int main(int argc, char const *argv[]) {
	struct timespec start_time, end_time;
	// get start timestamp
	clock_gettime(CLOCK_MONOTONIC, &start_time);
	
	N = atol(argv[1]);
	NUM_THREADS = atoi(argv[2]);
	
	// create and keep track of threads
	int thread_id[NUM_THREADS];
	pthread_t t[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++) {
		thread_id[i] = i;
		int result = pthread_create(&t[i], NULL, partialSum, (void*)&thread_id[i]);
		if (result != 0) {
			perror("CREATE THREAD ERROR");
			exit(EXIT_FAILURE);
		}
	}
	// accumulation
	long sum = 0;
	long *partial_sum;
	for (int i = 0; i < NUM_THREADS; i++) {
		long result = pthread_join(t[i], (void**)&partial_sum);
		if (result != 0) {
			perror("JOIN THREAD ERROR");
			exit(EXIT_FAILURE);
		}
		sum += *partial_sum;
	}
	free(partial_sum);

	// get end timestamp
	clock_gettime(CLOCK_MONOTONIC, &end_time);
	// calculate elapsed time
	double time_taken;
	time_taken = (end_time.tv_sec - start_time.tv_sec) * 1e9;
	time_taken = (time_taken + (end_time.tv_nsec - start_time.tv_nsec)) * 1e-9;
	
	// output results
	printf("Multithreading,%d,%ld,%ld,%lf\n", NUM_THREADS, N, sum, time_taken);
	
	return 0;
}
