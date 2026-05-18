#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// No parameters: compile all files and run each test once 
// One parameter: argv[1] is how many times to run each test
// Two parameters: argv[2]'s value doesn't matter , 
// just indicates to not include headers (appending data to another file using >>)

int main(int argc, char const *argv[]) {	
	int NUM_FILES = 4;
	const char object_filenames[4][8] = {"b.o", "mt.o", "mpf.o", "mpp.o"};
	const char header_filenames[4][32] = {"baseline.c", "multithreading.c", "multiprocessing_fork.c", "multiprocessing_popen.c"};	

	char command[256];

	int c; // how many times to run each test, default is once
	if (argc > 1) {
		// assumes files are already compiled
		// use > or >> to pipe output to another file (ex: output.csv)
		c = atoi(argv[1]);
	}
	else { // no parameters: compile all files and run each test once
		c = 1; 
		printf("Compiling c.files:\n");
		for (int i = 0; i < NUM_FILES; i++) {
			snprintf(command, sizeof(command), "gcc -o %s %s", object_filenames[i], header_filenames[i]);
			system(command);
		}


		printf("Executing test cases:\n");
	}	
	if (argc < 3) { // if another parameter is called, data should be appended (don't print headers)
		printf("File,Number of Threads/Processes,N,Sum,Time Taken\n");
		fflush(stdout);
	}
	
	char arg0[32];
	char arg1[32];
	char arg2[32];
	

	// sum up to n
	for (long n = 100000000; n <= 10000000000; n *= 10) {
		//printf("N = %d\n", n);
		snprintf(arg1, sizeof(arg1), "%ld", n);
		// how many threads/processes to run
		for (int m = 2; m <= 8; m *= 2) {
			//printf("NUM_PROCESSES or NUM_THREADS: %d\n", m);
			snprintf(arg2, sizeof(arg2), "%d", m);
			// which file to run
			for (int i = 0; i < NUM_FILES; i++) {		
				//printf("%s\n", object_filenames[i]);
				snprintf(arg0, sizeof(arg0), "%s", object_filenames[i]);
				// how many times the file will run
				for (int j = 0; j < c; j++) {
					if (i == 0) { // running baseline code
						snprintf(command, sizeof(command), "./%s %s", arg0, arg1);
						system(command);
					}
					else { // running multi- code
						snprintf(command, sizeof(command), "./%s %s %s", arg0, arg1, arg2);
						system(command);
					}
				}
			}
		}
	}	
	
	return 0;
}
