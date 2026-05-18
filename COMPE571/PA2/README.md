Experiment 1: Round Robin
    - rr.c
    takes five parameters
        - argv[1] = WORKLOAD1
        - argv[2] = WORKLOAD2
        - argv[3] = WORKLOAD3
        - argv[4] = WORKLOAD4
        - argv[5] = QUANTUM
    - experiment1.c
        Workload sizes:
        1 - 100000
        2 - 50000
        3 - 25000
        4 - 10000
        Quantum - variable - argv[1]
        File option - argv[2] : if empty append to file, else add headers
        Number of tests - 100

Experiment 2: Multi-Level Feedback Queue
     - mlfq.c
    takes five parameters
        - argv[1] = WORKLOAD1
        - argv[2] = WORKLOAD2
        - argv[3] = WORKLOAD3
        - argv[4] = WORKLOAD4
        - argv[5] = QUANTUM
    - experiment2.c
        Workload sizes:
        1 - 100000
        2 - 50000
        3 - 25000
        4 - 10000
        Quantum - variable - argv[1]
        File option - argv[2] : if empty append to file, else add headers
        Number of tests - 100

Experiment 3a: Performance - rr.c mlfq.c fcfs.c sjf.c
    - experiment3a.c
        Workload sizes:
        1 - 100000
        2 - 50000
        3 - 25000
        4 - 10000
        Number of tests - 100

Experiment 3b: Performance - rr.c mlfq.c fcfs.c sjf.c
    - experiment3b.c
        Workload sizes:
        1 - 100000
        2 - 100000
        3 - 100000
        4 - 100000
        Number of tests - 100

Generating results -  experiment1.c experiment2.c experiment3a.c experiment3b.c
    - compiling and executing each experiment will compile each of the dependent c files listed above
    - for experiment 1 and 2, include desired quantum value to test
    - for experiment 3a and 3b