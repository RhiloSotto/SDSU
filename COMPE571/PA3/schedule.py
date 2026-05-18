import sys
import math
import heapq

#NOTE: ALL PRINT STATEMENTS ARE FOR TESTING

# handles new arrival of tasks
class task_factory():
    def __init__(self, name, period, wcet):
        # input data
        self.name = name # task name
        self.period = int(period) # period
        self.wcet = [int(exec_time) for exec_time in wcet] # WCET @ 1188MHz, 918MHz, 648MHz, 384MHz

        # filtering data (fixed frequency scheduling)
        self.cpu_util = [exec_time / self.period for exec_time in self.wcet] # WCET/period

        # scheduling 
        self.next_instance_time = 0 # keep track of when next task arrives
        self.task = None # reference to child task

    # reset factory for new simulation
    def reset_factory(self):
        self.next_instance_time = 0
        self.task = None
    # creates fresh tasks
    def create_task(self, CPU_freq, current_time):
        if self.task is not None:
            # task already exists, missed deadline
            return
        else:
            self.next_instance_time += self.period
            # create task
            self.task = task_instance(self.name, self, CPU_freq, current_time, self.period, self.next_instance_time, self.wcet[CPU_freq])
            return self.task
    # clear upon task completion
    def clear_task(self):
        self.task = None
    

# handles task until completion
class task_instance():
    def __init__(self, name=None, parent=None, CPU_freq=None, arrival_time=None, period=None, deadline=None, remaining_time=None):
        # task data
        self.name = name
        self.parent = parent # reference to parent factory
        self.CPU_freq = CPU_freq # which CPU power to use
        self.period = period # check for RM scheduling
        self.arrival_time = arrival_time
        self.deadline = deadline # check for EDF scheduling
        self.remaining_time = remaining_time
    # move task back into ready queue
    def interrupt(self, start_time, end_time):
        self.remaining_time -= end_time - start_time
        # TODO: generate output message
    # tell parent that task completed
    def terminate(self):
        self.parent.clear_task()
        # TODO: generate output message
    # tie-break rule for heap
    def __lt__(self, other):
        # TODO: make RM vs EDF choice here?
        # RM uses period
        global schedule
        match schedule:
            case "RM":
                if self.period != other.period:
                    return self.period < other.period
            case "EDF":
                if self.deadline != other.deadline:
                    return self.deadline < other.deadline        
        # use name as tie-break
        return self.name < other.name


# Decision Points: Task Completion and Task Arrival
# hold all task factories and keep track of when tasks are going to arrive
task_manager = []

# argv[0] - input.txt file
# argv[1] - RM or EDF
# argv[2] - EE (optional)

# Needs to be known by task when making priority decisions
schedule = None
energy_efficient = None
best_case = None
best_energy_consumption = sys.maxsize

def main():
    # read arguments 
    argv = sys.argv[1:]
    # argument validation
    global schedule 
    global energy_efficient
    match len(argv):
        case 0:
            print("No arguments provided.")
            return
        case 1:
            print("No scheduling algorithm provided.")
            return
        case 2:
            input_file = argv[0]
            schedule = argv[1]  
        case 3:
            input_file = argv[0]
            schedule = argv[1]
            energy_efficient = argv[2]
        case _:
            print("Unexpected arguments.")
            return

    # print(f"Args: {argv}")
    # print(f"self: {argv[0]}")

    # system data
    num_tasks = 0
    max_sim_time = 0
    sim_time = 0
    CPU_freq = {0: 1188, 1: 918, 2: 648, 3: 384}
    CPU_power = []
    CPU_idle = 0

    # parse input#.txt file
    try:
        with open(input_file, 'r') as file:
            firstline = file.readline().rstrip('\n').split(' ')
            # print(f"{firstline}")
            num_tasks = int(firstline[0])
            max_sim_time = int(firstline[1])
            CPU_power = [int(power) for power in firstline[2:]]
            CPU_idle = int(firstline[6])

            for row in file:
                line = row.rstrip('\n').split(' ')
                # print(f"{line}")
                # create task factory for each task
                task_manager.append(task_factory(line[0], line[1], line[2:]))
    except FileNotFoundError:
        print(f"Error: File {input_file} not found.")
    except Exception as e:
        print(f"Error: {e}")

    # calculate hyperperiod (simulation time to guarantee feasibility)
    periods = []
    for factory in task_manager:
        periods.append(factory.period)
    hyperperiod = math.lcm(*periods)
    # print(hyperperiod)


    # keep track of what tasks have arrived
    ready_queue = []
    # track which task is currently running
    running_task = None
        
    # MAIN SCHEDULE LOOP (until one hyperperiod)
    # TODO: update ready queue for new tasks that will arrive (decision points)
    # pick next task
    # determine if decision points will effect current task - priority
    # progress/complete current task until decision point (completion, interrupt)

    # settings dictionary for each task
    settings = {}

    if energy_efficient == "EE":
        num_cases = pow(len(CPU_freq), num_tasks)
    else:
        num_cases = 1
    
    global best_case
    for case in range(0, num_cases, 1):
        ready_queue.clear()
        running_task = None
        for factory in task_manager:
            factory.reset_factory()

        i = 0
        cpu_util_sum = 0
        for factory in task_manager:
            settings[factory] = (case & (0x00000003 << i)) >> i
            # print(settings[factory], end=' ')
            cpu_util_sum += factory.cpu_util[settings[factory]]
            i += 2
        # if CPU utilization exceeds 100%, skip this case
        if cpu_util_sum > 1.0:
            # print(f"Case {case}: Skipped due to CPU overutilization ({cpu_util_sum:.2f})", end='\n')
            continue
        # print(i)

        # print(f"Case {case}: ", end='')
        if schedule_loop(ready_queue, running_task, settings, hyperperiod, CPU_freq, CPU_power, CPU_idle, False):
            # print(f"New best settings: {case}", end='\n')
            best_case = case
            # print(best_case)

    # final run with best settings
    if best_case is not None:
        ready_queue.clear()
        running_task = None
        for factory in task_manager:
            factory.reset_factory()

        i = 0
        for factory in task_manager:
                settings[factory] = (best_case & (0x00000003 << i)) >> i
                # print(settings[factory], end=' ')
                cpu_util_sum += factory.cpu_util[settings[factory]]
                i += 2

        schedule_loop(ready_queue, running_task, settings, max_sim_time, CPU_freq, CPU_power, CPU_idle, True)
    else:
        print("No feasible scheduling found.")

def create_tasks(ready_queue, sim_time, settings):
    global task_manager
    for factory in task_manager:
        if sim_time >= factory.next_instance_time:
            task = factory.create_task(settings[factory], sim_time)
            if task is None:
                print(f"{factory.name} failed to meet it's deadline at time {sim_time}")
                return False
            heapq.heappush(ready_queue, task)
    return True

def schedule_loop(ready_queue, running_task, settings, sim_end_time, CPU_freq, CPU_power, CPU_idle, output):
    # current time in the simulation
    sim_time = 0
    # minimum amongst all factory next_instance_time
    # future time in the simulation
    next_arrival_time = sys.maxsize
    # when task began execution
    # past time in the simulation
    execution_start_time = 0
    termination_time = sys.maxsize
    # output should print current time, which task, then run to what point
    total_energy_consumption = 0
    idle_sum = 0
    idle_percentage = 0
    while sim_time < sim_end_time:
    #while sim_time < max_sim_time:
        create_tasks(ready_queue, sim_time, settings)
        
        next_arrival_time = sys.maxsize
        for factory in task_manager:
            next_arrival_time = min(next_arrival_time, factory.next_instance_time)

        # CPU IDLE
        if len(ready_queue) == 0:
            running_task = None
            # print("CPU_IDLE", end=' ')
            execution_start_time = sim_time
            sim_time = next_arrival_time
        # TASK RUNNING
        else:
            running_task = heapq.heappop(ready_queue)
            # print(running_task.name, end=' ')
            execution_start_time = sim_time
            termination_time = sim_time + running_task.remaining_time
            # tasks are created during task runtime
            while next_arrival_time < termination_time:
                sim_time = next_arrival_time
                # print(f"loop{sim_time}", end=' ')
                # have tasks arrive
                if not create_tasks(ready_queue, sim_time, settings):
                    return False
                # make decision, interrupt or continue until termination
                if ready_queue[0] < running_task:
                    break
                # find next arrival time
                next_arrival_time = sys.maxsize
                for factory in task_manager:
                    next_arrival_time = min(next_arrival_time, factory.next_instance_time)

            # task was interrupted
            if next_arrival_time < termination_time:
                sim_time = next_arrival_time
                # print(f"{sim_time}, inter")
                running_task.interrupt(execution_start_time, sim_time)
                # place back into ready queue
                heapq.heappush(ready_queue, running_task)
                # running_task = None
            # task completed without interruption
            else:
                sim_time = termination_time
                # print(f"{sim_time}, term")
                running_task.terminate()
                # running_task = None

        # if simulation ends before task completes
        if sim_time > sim_end_time:
            # print("here")
            sim_time = sim_end_time
        run_time = sim_time - execution_start_time
        if running_task:
            energy_consumption = CPU_power[running_task.CPU_freq] * run_time / 1000
            if output:
                print(f"{execution_start_time} {running_task.name} {CPU_freq[running_task.CPU_freq]} {run_time} {energy_consumption}")
        else:
            energy_consumption = CPU_idle * run_time / 1000
            idle_sum += run_time
            if output:
                print(f"{execution_start_time} IDLE IDLE {run_time} {energy_consumption}")
        total_energy_consumption += energy_consumption
    idle_percentage = (idle_sum / sim_end_time) * 100
    if output:
        print(f"{total_energy_consumption:.2f} {idle_percentage:.2f}% {sim_end_time - idle_sum}")

    global best_energy_consumption
    if output:
        return True
    elif total_energy_consumption <= best_energy_consumption:
        # print("here")
        best_energy_consumption = total_energy_consumption
        return True
        
    return False

main()