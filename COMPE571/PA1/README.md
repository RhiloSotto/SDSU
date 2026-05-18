CASE 1: BASELINE - baseline.c
    takes either one or two parameters
    If only one parameter:
        - argv[1] = N
    If two parameters:
        - argv[1] = N_start
        - argv[2] = N_end

CASE 2: MULTITHREADING - multithreading.c
    takes two parameters:
        - argv[1] = N
        - argv[2] = NUM_THREADS
CASE 3: MULTITASKING
    OPTION 1: Creating multiple tasks via fork() - multiprocessing_fork.c
        takes two parameters:
            - argv[1] = N
            - argv[2] = NUM_TASKS
    OPTION 2: Creating multiple tasks via popen() - multiprocessing_popen.c
        takes two parameters:
            - argv[1] = N
            - argv[2] = NUM_TASKS

Generating results - testcase.c
    takes up to three parameters:
    If no parameters:
        - compiles baseline.c, multithreading.c, multiprocessing_fork.c, multiprocessing_popen.c
        - generates object files b.o, mt.o, mpf.o, mpp.o corresponding to each c file
        - and runs each test case required:
            - N = {100000000, 1000000000, 10000000000} 
            - NUM_THREADS/NUM_TASKS = {2, 4, 8}
    If one parameter:
        - skips compiling c files
        - argv[1] = number of tests for each test case
        - includes headers for writing '>' to a new .csv file
    If two parameters:
        - skips compiling c files
        - argv[1] = number of tests for each test case
        - argv[2] value doesn't matter, only indicates to skip header
        - skips adding headers for appending '>>' to an existing .csv file