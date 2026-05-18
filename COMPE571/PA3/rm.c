#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// number of active CPU settings
#define CPU_SETTINGS 4


typedef struct task{
	char name[8]; // task name
	int period; // period
	int wcet[4]; // WCET @ 1188MHz, 918MHz, 648MHz, 384MHz
	double cpu_util[4]; // WCET/period
	int arrival_time;
	int deadline;
	int remaining_time;
} task;

int int_pow(int base, int exponent) {
	int result = 1;
	for (int i = 0; i < exponent; ++i) {
		result *= base;
	}
	return result;
}

// argv[0] - filename
// argv[1] - input.txt file
// argv[2] - RM or EDF
// argv[3] - EE (optional)

int main(int argc, char const *argv[]) {
	char const *input_file = argv[1];
	char const *scheduler = argv[2];
	char const *energy_efficient = argv[3];
	
    // TODO: open and parse txt file

    FILE *fptr;
    fptr = fopen(input_file, "r");
	//printf("file read\n");
	//fflush(stdout);
    
    int num_tasks = 0;
    int sim_time = 0;
    int CPU_power[CPU_SETTINGS];
    int CPU_idle = 0;

    if (fptr == NULL) {
		printf("file not opened");
        return -1;
    }
    else {
        // first row reads: 
        // # of tasks
        // time to execute up to (when printing results)
        fscanf(fptr, "%d %d", &num_tasks, &sim_time);
        // active CPU power @ 1188MHz
        // active CPU power @ 918MHz
        // active CPU power @ 648MHz
        // active CPU power @ 384MHz
        for (int i = 0; i < CPU_SETTINGS; ++i) {
            fscanf(fptr, "%d", &CPU_power[i]);
        }
        // idle CPU power
        fscanf(fptr, "%d", &CPU_idle);
    }
	
	// checking that first row was read
	printf("%d %d ", num_tasks, sim_time);
	for (int i = 0; i < CPU_SETTINGS; ++i) {
		printf("%d ", CPU_power[i]);
	}
	printf("%d\n", CPU_idle);
	
	
    // following rows:
    // task name
    // deadline/period
    // WCET @ 1188MHz
    // WCET @ 918MHz
    // WCET @ 648MHz
    // WCET @ 384MHz
    task *task_list = (task*)malloc(num_tasks * sizeof(task));
    
    for (int i = 0; i < num_tasks; ++i) {
		// read task name and period
		fscanf(fptr, "%s %d", &task_list[i].name, &task_list[i].period);
		// read WCET and calculate CPU utilization for each setting
		for (int j = 0; j < CPU_SETTINGS; ++j) {
			fscanf(fptr, "%d", &task_list[i].wcet[j]);
			task_list[i].cpu_util[j] = 
				(double)task_list[i].wcet[j]/task_list[i].period;
		}
		// read task name and period
		printf("%s %d\n", task_list[i].name, task_list[i].period);
		// read WCET and calculate CPU utilization for each setting
		for (int j = 0; j < CPU_SETTINGS; ++j) {
			printf("%d, %lf\n", task_list[i].wcet[j], 
				task_list[i].cpu_util[j]);
		}
	}	
	// close input file
    fclose(fptr);

	//printf("here");
	//fflush(stdout);
    // NOTE: regardless of RM or EDF, max CPU utilization is 1
	double total_util = 0.0;
	int cases = int_pow(CPU_SETTINGS, num_tasks);
	int settings[num_tasks];
	int valid_cases = 0;
	for (int i = 0; i < cases; ++i) {
		// each setting is base 4, bitmask and shift
		settings[0] = (i & 0x00000003) >> 0;
		settings[1] = (i & 0x0000000C) >> 2;
		settings[2] = (i & 0x00000030) >> 4;
		settings[3] = (i & 0x000000C0) >> 6;
		settings[4] = (i & 0x00000300) >> 8;
		
		//printf("%d %d %d %d %d\n", settings[4], settings[3], 
			//settings[2], settings[1], settings[0]);
		//fflush(stdout);		
		total_util = 0.0;
		for (int j = 0; j < num_tasks; ++j) {
			total_util += task_list[j].cpu_util[settings[j]];
		}
		// CPU utilization <= 1
		if (total_util < 1.0) {
			printf("Case: %d | %lf\n", i + 1, total_util);
			valid_cases += 1;
		}
		fflush(stdout);
	}
	printf("Valid Cases: %d\n", valid_cases);
	
    // No deadlines missed --> simulate schedule for one hyperperiod
    // TODO: simulate RM, EDF schedule
    
    
    
    // NOTE: for EE schedules
    // power consumption must be more efficient




    // TODO: print scheduling up to t time units
    // time started
    // task name
    // CPU frequency task runs at
    // how long it ran for
    // energy consumed in Joules

    // after t time units
    // total energy consumption
    // percentage of time spend idle
    // total system execution time

    // free dynamic array of tasks
    free(task_list);
    return 0;
}
