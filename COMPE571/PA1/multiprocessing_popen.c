// multiprocessing_popen.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char const *argv[]) {
	struct timespec start_time, end_time;
	// get start timestamp
	clock_gettime(CLOCK_MONOTONIC, &start_time);

	long N = atol(argv[1]);
	int NUM_PROCESSES = atoi(argv[2]);

	// file pointer to processes,
	// command string to run,
	// and buffer for return values (partial sums)
	FILE *fp[NUM_PROCESSES];
	char command[256];
	char buffer[NUM_PROCESSES][1024];
	char arg0[32];
	snprintf(arg0, sizeof(arg0), "./b.o");
	char arg1[32];
	char arg2[32];
	
	// divide workload
	long quotient = N / NUM_PROCESSES;
	long remainder = N % NUM_PROCESSES;
	// div_t result = div(N, NUM_PROCESSES);
	
	// create processes
	long N_start = 0, N_end = 0;
	for (int i = 0; i < NUM_PROCESSES; i++) {
		N_start = N_end;
		N_end += quotient;
		// the last created process
		// will be responsible for
		// summing the remainder
		if (i == NUM_PROCESSES - 1) {
			N_end += remainder;
		}
		
		// convert ints to parsable strings
		snprintf(arg1, sizeof(arg1), "%ld", N_start);
		snprintf(arg2, sizeof(arg2), "%ld", N_end);
		snprintf(command, sizeof(command), "%s %s %s", arg0, arg1, arg2);
	
		// read mode
		fp[i] = popen(command, "r");
	}
	
	// accumulation
	long sum = 0;
	for (int i = 0; i < NUM_PROCESSES; i++) {
		// read output from file
		fgets(buffer[i], sizeof(buffer[i]), fp[i]);
		sum += atol(buffer[i]);
		int status = pclose(fp[i]);
		if (status == -1) {
			perror("CLOSE PIPE ERROR");
		}
	}

	// get end timestamp
	clock_gettime(CLOCK_MONOTONIC, &end_time);
	// calculate elapsed time
	double time_taken;
	time_taken = (end_time.tv_sec - start_time.tv_sec) * 1e9;
	time_taken = (time_taken + (end_time.tv_nsec - start_time.tv_nsec)) * 1e-9;
	
	// output results
	printf("Multiprocessing_popen,%d,%ld,%ld,%lf\n", NUM_PROCESSES, N, sum, time_taken);
	
	return 0;
}
