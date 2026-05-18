import sys
import random

# page table entries
class Entry():
    def __init__(self):
        self.translation = None # 5 bit VPN --> PPN
        self.dirty = bool(False) # 1 bit written to?
        self.referenced = bool(False) # 1 bit Referenced?
        self.auxillary = 0 # 9 bits of extra info
    def clear(self):
        self.translation = None
        self.dirty = bool(False)
        self.referenced = bool(False)
        self.auxillary = 0
    def __str__(self):
        return f"{self.translation}, {self.dirty}, {self.referenced}, {self.auxillary}"

# 512B page size
# addresses are 16 bits
# 9 bits of offset
class Process():
    def __init__(self):
        self.page_table = [Entry() for i in range(128)] # = 2^7 entries

# dictionary of all processes in input
# key = process id, maps to process object
process_list = {}

# which page replacement algorithm to use
algorithm = None

# keeps track of what entries are in the main memory
main_memory_buffer = [None] * 32

sim_time = 0
# how many RAND trials to use
num_sims = 0
# statistics to keep track of
total_page_faults = 0
total_disk_references = 0
total_dirty_page_writes = 0

def main():
    global sim_time
    global num_sims
    global total_page_faults
    global total_disk_references
    global total_dirty_page_writes
    # read arguments
    argv = sys.argv[1:]
    # argument validation
    global algorithm
    match len(argv):
        case 2:
            input_file = argv[0]
            algorithm = argv[1]  
        case 3:
            input_file = argv[0]
            algorithm = argv[1]
            num_sims = int(argv[2])
        case _:
            print("Unexpected arguments:")
            print("page_replacement.py input_file.txt [RAND, FIFO, LRU, PER, MIN] [# RAND Trials]")
            return
    try:
        with open(input_file, 'r') as file:
            for row in file:
                line = row.rstrip('\n').split('\t')
                # print(f"{line}")
                process_id = int(line[0])
                virtual_address = int(line[1])
                access_type = str(line[2])

                # create process if does not exist
                if process_id not in process_list:
                    # print(f"Creating process {process_id}")
                    process_list[process_id] = Process()

                # go to process's entry with VPN as page table index
                process = process_list[process_id]
                VPN = get_VPN(virtual_address)
                entry = process.page_table[VPN]
                # check if entry has a valid translation
                if entry.translation is None:                    
                    # print("Invalid - PAGE FAULT")
                    # TODO: handle page fault
                    page_replacement_algorithm(entry)
                    # FIFO uses only initial time
                    match(algorithm):
                        case "FIFO":
                            entry.auxillary = sim_time
                        case "LFU":
                            entry.auxillary += 1
                    

                # after handling page fault, page is valid
                # print("Valid")
                if access_type == "W":
                    entry.dirty = True
                entry.referenced = True
                # LRU updates entry with new access time
                # auxillary information for each algorithm
                match(algorithm):
                    case "RAND":
                        pass
                    case "FIFO":
                        pass
                    case "LRU":
                        entry.auxillary = sim_time
                    case "PER":
                        if sim_time % 200 == 0:
                            # print(f"RESET - {sim_time}")
                            for page in main_memory_buffer:
                                if page:
                                    # print(page)
                                    page.referenced = False
                    case "LFU":
                        # keep count of how many references
                        entry.auxillary += 1
                    case "MIN":
                        # find next access time for this entry
                        pass

                sim_time += 1

    except FileNotFoundError:
        print(f"Error: File {input_file} not found.")
    except Exception as e:
        print(f"Error: {e}")

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
    # global sim_time
    global total_page_faults
    global total_disk_references
    global total_dirty_page_writes

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
        case "LFU":
            victim = min(enumerate(main_memory_buffer), key = lambda s: (s[1].auxillary, s[0]))[0]
        case "MIN":
            victim = min(enumerate(main_memory_buffer), key = lambda s: (s[1].auxillary, s[0]))[0]
        case _:
            pass
    # update statistics and replace page in main memory
    if main_memory_buffer[victim].dirty:
        total_disk_references += 1
        total_dirty_page_writes += 1
    main_memory_buffer[victim].clear()
    entry.translation = victim
    main_memory_buffer[victim] = entry

    
random.seed(1)
main()
if algorithm == "RAND":
    pass


print(algorithm, total_page_faults, total_disk_references, total_dirty_page_writes)

print("--   Done    --")