#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// Workload sizes:
// 1 - 100000
// 2 - 50000
// 3 - 25000
// 4 - 10000
// Quantum - variable - argv[1]
// File option - argv[2] : if empty append to file, else add headers
// Number of tests - 100

int main(int argc, char const *argv[]) {	
	int NUM_FILES = 1;
	const char object_filenames[1][8] = {"rr.o"};
	const char header_filenames[1][8] = {"rr.c"};	

	char command[256];

	// each case will have 100 trials
	if (argc == 1) { // no parameters: compile file and exit
		printf("Compiling c.files:\n");
		for (int i = 0; i < NUM_FILES; i++) {
			snprintf(command, sizeof(command), "gcc -o %s %s", object_filenames[i], header_filenames[i]);
			system(command);
		}
		return 0;
	}	
	// use > command for first case, otherwise >> for appending to output1.csv
	if (argc == 3) { // data should include headers (use for first case)
		printf("File,Tr1,Tr2,Tr3,Tr4,Num CS,CS Time,Quantum\n");
		fflush(stdout);
	}
	
	// filename
	char arg0[32];
	snprintf(arg0, sizeof(arg0), "%s", object_filenames[0]);
	// workload
	char arg_workload[128] = "100000 50000 25000 10000";
	// quantum size
	char arg_quantum[32];
	snprintf(arg_quantum, sizeof(arg_quantum), "%s", argv[1]);
	
	// run command 100 times through terminal
	snprintf(command, sizeof(command), "./%s %s %s", arg0, arg_workload, arg_quantum);
	for (int i = 0; i < 100; i++) {
		system(command);
	}
	
	return 0;
}
