#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// Workload sizes:
// 1 - 100000
// 2 - 50000
// 3 - 25000
// 4 - 10000
// Quantum - already determined
// Number of tests - 100

int main(int argc, char const *argv[]) {	
	int NUM_FILES = 4;
	const char object_filenames[4][8] = {"rr.o", "mlfq.o", "fcfs.o", "sjf.o"};
	const char header_filenames[4][8] = {"rr.c", "mlfq.c", "fcfs.c", "sjf.c"};	

	char command[256];
	
	for (int i = 0; i < NUM_FILES; i++) {
		snprintf(command, sizeof(command), "gcc -o %s %s", object_filenames[i], header_filenames[i]);
		system(command);
	}	
	// use > command for output3b.csv
	printf("File,Tr1,Tr2,Tr3,Tr4,Num CS,CS Time,Quantum\n");
	fflush(stdout);
	
	// workload
	char arg_workload[128] = "100000 100000 100000 100000";
	// quantum sizes found for RR and MLFQ (ignored for FCFS and SJF)
	char arg_quantum[4][32] = {"5000", "25000", "0", "0"};
	
	for (int i = 0; i < 4; i++) {
		// file and quantum changing
		snprintf(command, sizeof(command), "./%s %s %s", object_filenames[i], arg_workload, arg_quantum[i]);
		// run command 100 times through terminal
		for (int trial = 0; trial < 10; trial++) {
			system(command);
		}
	}
	
	return 0;
}
