// baseline.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


int main(int argc, char const *argv[]) {
	struct timespec start_time, end_time;
	// get start timestamp
	clock_gettime(CLOCK_MONOTONIC, &start_time);
	
	// argv[1]
	long N_start;
	// argv[2]
	long N_end;
	// user only provided N to count up to
	if (argc > 2) {
		N_start = atol(argv[1]);
		N_end = atol(argv[2]);
	}
	else {
		N_start = 0;
		N_end = atol(argv[1]);
	}
	// prevent infinity loop, end val less than start val
	if (N_end < N_start) {
		perror("Invalid Inputs");
		exit(EXIT_FAILURE);
	}

	// summation
	long sum = N_start;
	for (long i = N_start + 1; i < N_end; i += 1) {
		sum += i;
	}

	// get end timestamp
	clock_gettime(CLOCK_MONOTONIC, &end_time);
	// calculate elapsed time
	double time_taken;
	time_taken = (end_time.tv_sec - start_time.tv_sec) * 1e9;
	time_taken = (time_taken + (end_time.tv_nsec - start_time.tv_nsec)) * 1e-9;
	
	if (argc < 3) {
		printf("Baseline,1,%ld,%ld,%lf\n", N_end, sum, time_taken);
	}
	// ran from popen (provided start and end val)
	else {
		printf("%ld", sum);
	}	
	return 0;
}
