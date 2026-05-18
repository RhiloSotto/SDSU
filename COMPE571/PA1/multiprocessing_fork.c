// multiprocessing_fork.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// argv[1] is N
// argv[2] is NUM_PROCESSES
int main(int argc, char const *argv[]) {
	struct timespec start_time, end_time;
	// get start timestamp
	clock_gettime(CLOCK_MONOTONIC, &start_time);
	
	long N = atol(argv[1]);
	int NUM_PROCESSES = atoi(argv[2]);
	
	// determine how many times to fork
	// and therefore how many processes are made:
	// only powers of 2 are supported
	int NUM_FORKS = 0;
	while (NUM_PROCESSES > 1) {
		NUM_PROCESSES >>= 1;
		NUM_FORKS += 1;
	}
	NUM_PROCESSES = (1 << NUM_FORKS);

	// pipe file descriptors
	int pipes[NUM_PROCESSES][2];
	// create pipes
	for (int i = 0; i < NUM_PROCESSES; i++) {
		if (pipe(pipes[i]) == -1) {
			perror("PIPE ERROR");
			exit(EXIT_FAILURE);
		}
	}

	// give each task a unique identifier
	int task_id = 0;
	// store fork return values
	pid_t pid[NUM_FORKS];
	// create forks
	for (int i = 0; i < NUM_FORKS; i++) {
		pid[i] = fork();
		if (pid[i] == -1) {
			perror("FORK ERROR");
			exit(EXIT_FAILURE);
		}
		// parent (1) or child (0)
		task_id += (pid[i] > 0) ? (1 << i) : 0;
	}	

	// arbitrarily designating task_id 0 as the accumulator
	// accumulator does not need to write
	// all other processes only need to write for itself
	// close all other write ends
	for (int i = 0; i < NUM_PROCESSES; i++) {
		if (i != task_id || task_id == 0) {
			close(pipes[i][1]);
		}
	}
	// only accumulator will need to read
	// accumulator doesn't need to read itself
	// close read ends for all other processes
	if (task_id == 0) {
		close(pipes[0][0]);
	}
	for (int i = 0; i < NUM_PROCESSES; i++) {
		if (task_id != 0) {
			close(pipes[i][0]);
		}
	}
	
	// (partial) summation
	long sum = 0;
	for (long i = task_id; i < N; i += NUM_PROCESSES) {
		sum += i;
	}
	
	
	if (task_id == 0) {
		// accumulation
		long partial_sum;
		for (int i = 1; i < NUM_PROCESSES; i++) {
			ssize_t bytes_read = read(pipes[i][0], &partial_sum, sizeof(partial_sum));
			if (bytes_read == -1) {
				perror("READ_ERROR");
				exit(EXIT_FAILURE);
			}
			sum += partial_sum;
			close(pipes[i][0]);
		}
		// get end timestamp
		clock_gettime(CLOCK_MONOTONIC, &end_time);
		// calculate elapsed time
		double time_taken;
		time_taken = (end_time.tv_sec - start_time.tv_sec) * 1e9;
		time_taken = (time_taken + (end_time.tv_nsec - start_time.tv_nsec)) * 1e-9;
	
		// output results
		printf("Multiprocessing_fork,%d,%ld,%ld,%lf\n", NUM_PROCESSES, N, sum, time_taken);
	}
	else {
		ssize_t bytes_write = write(pipes[task_id][1], &sum, sizeof(sum));
		if (bytes_write == -1) {
			perror("WRITE_ERROR");
			exit(EXIT_FAILURE);
		}
		close(pipes[task_id][1]);
	}
	
	return 0;
}
