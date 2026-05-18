import sys
import random
import csv

# page table entries
class Entry():
    def __init__(self):
        self.translation = None # 5 bit VPN --> PPN
        self.dirty = False # 1 bit written to?
        self.referenced = False # 1 bit Referenced?
        self.auxillary = None # 9 bits of extra info
    def clear(self):
        self.translation = None
        self.dirty = False
        self.referenced = False
        self.auxillary = None
    def __str__(self):
        return f"{self.translation}, {self.dirty}, {self.referenced}, {self.auxillary}"

# 512B page size
# addresses are 16 bits
# 9 bits of offset
class Process():
    def __init__(self):
        self.page_table = [Entry() for i in range(128)] # = 2^7 entries

# store all references from input file
class Future():
    def __init__(self):
        self.length = 0
        self.process_id = []
        self.VPN = []
        self.access_type = []
future = Future()
# dictionary of all processes in input
# key = process id, maps to process object
process_list = {}
# which page replacement algorithm to use
# RAND, FIFO, LRU, PER, MIN
algorithm = None
input_file = ""
# how many times to run the simulation (for RAND trials)
num_sims = 1

# keeps track of what entries are in the main memory
main_memory_buffer = [None] * 32
# statistics to keep track of
total_page_faults = 0
total_disk_references = 0
total_dirty_page_writes = 0


def read_inputs():
    global algorithm
    global num_sims
    global input_file
    # read arguments
    argv = sys.argv[1:]
    # argument validation
    match len(argv):
        case 2:
            input_file = argv[0]
            algorithm = argv[1]  
        case 3:
            input_file = argv[0]
            algorithm = argv[1]
            num_sims = int(argv[2])
        case _:
            print("Expected arguments:")
            print("page_replacement.py input_file.txt [RAND, FIFO, LRU, PER, MIN] [# Trials]")
            return
    try:
        with open(input_file, 'r') as file:
            for row in enumerate(file):
                line = row[1].rstrip('\n').split('\t')
                # print(f"{line}")
                process_id = int(line[0])
                virtual_address = int(line[1])
                access_type = str(line[2])

                # create process if does not exist
                if process_id not in process_list:
                    # print(f"Creating process {process_id}")
                    process_list[process_id] = Process()
                future.length += 1
                future.process_id.append(process_id)
                future.VPN.append(get_VPN(virtual_address))
                future.access_type.append(access_type)
    except FileNotFoundError:
        print(f"Error: File {input_file} not found.")
    except Exception as e:
        print(f"Error: {e}")
    return


def simulate():
    global total_page_faults
    global total_disk_references
    global total_dirty_page_writes
    global main_memory_buffer
    global algorithm

    for i in range(0, future.length):
        # go to process's entry with VPN as page table index
        process = process_list[future.process_id[i]]
        process_id = future.process_id[i]
        VPN = future.VPN[i]
        entry = process.page_table[VPN]
        # check if entry has a valid translation
        if entry.translation is None:                    
            # print("Invalid - PAGE FAULT")
            # handle page fault
            page_replacement_algorithm(entry)
            # default auxillary info is initial access time
            entry.auxillary = i
          
        # after handling page fault, page is valid and can now be referenced
        # print("Valid")
        if future.access_type[i] == "W":
            entry.dirty = True
        entry.referenced = True

        # auxillary information for each algorithm
        match(algorithm):
            case "RAND":
                pass
            case "FIFO":
                pass
            case "LRU":
                entry.auxillary = i
            case "PER":
                # 200th entries (first entry is i == 0)
                if (i + 1) % 200 == 0:
                    for page in main_memory_buffer:
                        if page:
                            page.referenced = False
            case "MIN":
                # find next access time for this entry (that isn't the present)
                for j in range(i + 1, future.length):
                    # print(f"looking... {j}, {future.process_id[j]} - {process_id}, {future.VPN[j]} - {VPN}")
                    if future.process_id[j] == process_id and future.VPN[j] == VPN:
                        entry.auxillary = j
                        # print("future hit")
                        break


# translate the virtual address to a physical address
def get_VPN(v_address: int):
    VPN = v_address >> 9 # get 7 MSB
    # offset = v_address & 0x01FF # get 9 LSB
    # print(VPN, offset, end=" ")
    return VPN

# return a free page location in main memory
# if all pages are full, use algorithm to clean a location
# handle dirty page writes, and auxillary information
def page_replacement_algorithm(entry: Entry):
    global total_page_faults
    global total_disk_references
    global total_dirty_page_writes
    global main_memory_buffer
    global algorithm

    total_page_faults += 1
    total_disk_references += 1
    # find empty page
    for index, reference in enumerate(main_memory_buffer):
        if reference is None:
            # print(f"free: {index}")
            entry.translation = index
            main_memory_buffer[index] = entry
            return
    # replace a page
    match(algorithm):
        case "RAND":
            victim = random.randint(0, 31)
        case "FIFO":
            victim = min(enumerate(main_memory_buffer), key = lambda s: (s[1].auxillary, s[0]))[0]
        case "LRU":
            victim = min(enumerate(main_memory_buffer), key=lambda s: (s[1].auxillary, s[1].dirty, s[0]))[0]
        case "PER":
            victim = min(enumerate(main_memory_buffer), key = lambda s: (s[1].referenced, s[1].dirty, s[0]))[0]
        case "MIN":
            victim = min(enumerate(main_memory_buffer), key = lambda s: (-s[1].auxillary, s[0]))[0]
        case _:
            pass
    # update statistics and replace page in main memory
    if main_memory_buffer[victim].dirty:
        total_disk_references += 1
        total_dirty_page_writes += 1
    main_memory_buffer[victim].clear()
    entry.translation = victim
    main_memory_buffer[victim] = entry

# reset for another simulation run
def reset():
    global total_page_faults
    global total_disk_references
    global total_dirty_page_writes
    global main_memory_buffer
    total_page_faults = 0
    total_disk_references = 0
    total_dirty_page_writes = 0
    for entry in main_memory_buffer:
        entry.clear()
    main_memory_buffer = [None] * 32
    
read_inputs()

results = []
with open('results.csv', mode='w', newline='') as results_file:
    print("-- Starting Simulation --")
    results_writer = csv.writer(results_file)
    headers = ['Seed', 'Input File', 'Algorithm', 'Total Page Faults', 'Total Disk References', 'Total Dirty Page Writes']
    results_writer.writerow(headers)
    print(headers)
    for i in range(0, num_sims):
        random.seed(i)
        simulate()
        # report results
        result = [i, input_file, algorithm, total_page_faults, total_disk_references, total_dirty_page_writes]
        results_writer.writerow(result)
        print(result)
        reset()

print("-- Done --")