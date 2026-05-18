#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/wait.h> 
#include <string.h> 
#include <time.h> 
#include <signal.h>
#include <sys/time.h>


/************************************************************************************************ 
					DO NOT CHANGE THE FUNCTION IMPLEMENTATION
*************************************************************************************************/
void myfunction(int param){

	int i = 2;
	int j, k;

	while(i < param){
		k = i; 
		for (j = 2; j <= k; j++)
		{
			if (k % j == 0){
				k = k/j;
				j--;
				if (k == 1){
					break;
				}
			}
		}
		i++;
	}
}
/************************************************************************************************/

/************************************************************************************************ 
		These argv statements represent the workload size of each task and 
		the time quantum value for Round Robin scheduling for each task.
*************************************************************************************************/

// argv[0] - filename
// argv[1] - WORKLOAD1
// argv[2] - WORKLOAD2
// argv[3] - WORKLOAD3
// argv[4] - WORKLOAD4
// argv[5] - QUANTUM

int main(int argc, char const *argv[]) {
	pid_t pid[4];
	int running[4];
	int WORKLOAD[4];

    // read argv
    for (int i = 0; i < 4; i++) {
        WORKLOAD[i] = atoi(argv[i + 1]);
    }
    int QUANTUM = atoi(argv[5]);

    for (int i = 0; i < 4; i++) {
        pid[i] = fork();
        // create child process
        if (pid[i] == 0) {
            myfunction(WORKLOAD[i]);
            exit(0);
        }
        // suspend child process
        kill(pid[i], SIGSTOP);
        if (WORKLOAD[i] > 0) {
            running[i] = 1;
        }
    }
    
    // arrival time
	struct timespec arrival_time;
    // timestamp after process is terminated
	struct timespec completion_time[4];
    // elapsed time for each process
    double turnaround_time[4];
    // all processes arrive at the same time
	clock_gettime(CLOCK_MONOTONIC, &arrival_time);

    // keep track of context switching
    // the first task is not considered a context switch
    // and the last task completing is not a context switch
	int num_cs = -1;
    // get a timestamp before SIGSTOP and after SIGCONT
    struct timespec cs_start;
	struct timespec cs_end;
    // accumulates cs time
    double total_cs_time = 0.0;
    double cs_time = 0.0;

	/************************************************************************************************ 
		At this point, all  newly-created child processes are stopped, and ready for scheduling.
	*************************************************************************************************/


	/************************************************************************************************
		- Scheduling code starts here
		- Below is Round Robin
	************************************************************************************************/

	while (running[0] > 0 || running[1] > 0 || running[2] > 0 || running[3] > 0) {
		for (int i = 0; i < 4; i++) {
            // TASK EXECUTION
            if (running[i] > 0) {
                // resume task
                kill(pid[i], SIGCONT);

                // calculate context switch time
                // NOTE: this itself causes additional wait time
                num_cs += 1;
                // code will only execute from second task onwards
                if (num_cs > 0) {
                    // context switch end
                    clock_gettime(CLOCK_MONOTONIC, &cs_end);
                    // calculate
                    cs_time = (cs_end.tv_sec - cs_start.tv_sec) * 1e9;
                    cs_time = (cs_time + (cs_end.tv_nsec - cs_start.tv_nsec)) * 1e-9;
                    // accumulate
                    total_cs_time += cs_time;
                }

                // wait for time-quantum
                usleep(QUANTUM);

                waitpid(pid[i], &running[i], WNOHANG);
                // context switch start
                clock_gettime(CLOCK_MONOTONIC, &cs_start);
                if (running[i] > 0) {
                    // pause task
                    kill(pid[i], SIGSTOP);
                }
                else {
                    // task finish time
                    clock_gettime(CLOCK_MONOTONIC, &completion_time[i]);
                }
            }
        }
	}

	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/

	/************************************************************************************************ 
		At this point, all processes are finished, and can have their turnaround time calculated.
	*************************************************************************************************/
	
    for (int i = 0; i < 4; i++) {
        // calculate elapsed time for each task
		turnaround_time[i] = (completion_time[i].tv_sec - arrival_time.tv_sec) * 1e9;
		turnaround_time[i] = (turnaround_time[i] + (completion_time[i].tv_nsec - arrival_time.tv_nsec)) * 1e-9;
    }

    // report results
    // filename, Tr1, Tr2, Tr3, Tr4, #cs, Tcs, Quantum\n
    printf("RR,%lf,%lf,%lf,%lf,%d,%lf,%d\n", turnaround_time[0],turnaround_time[1],turnaround_time[2],turnaround_time[3],num_cs,total_cs_time,QUANTUM);

	return 0;
}