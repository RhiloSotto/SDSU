import sys


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

class Future():
    def __init__(self):
        self.process_id = []
        self.VPN = []
        self.access_type = []

future = Future()

# dictionary of all processes in input
# key = process id, maps to process object
process_list = {}

# keeps track of what entries are in the main memory
main_memory_buffer = [None] * 32

sim_time = 0
# statistics to keep track of
total_page_faults = 0
total_disk_references = 0
total_dirty_page_writes = 0

def main():
    global sim_time
    global total_page_faults
    global total_disk_references
    global total_dirty_page_writes

    # read arguments
    argv = sys.argv[1:]
    # argument validation
    global algorithm
    match len(argv):
        case 0:
            print("No arguments provided.")
            return
        case 1:
            input_file = argv[0]
        case _:
            print("Unexpected arguments.")
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
                future.process_id.append(process_id)
                future.VPN.append(get_VPN(virtual_address))
                future.access_type.append(access_type)

    except FileNotFoundError:
        print(f"Error: File {input_file} not found.")
    except Exception as e:
        print(f"Error: {e}")

    for i in range(0, len(future.process_id)):
        # print(i)
        # go to process's entry with VPN as page table index
        process = process_list[future.process_id[i]]
        process_id = future.process_id[i]
        VPN = future.VPN[i]
        entry = process.page_table[VPN]
        # check if entry has a valid translation
        if entry.translation is None:                    
            # print("Invalid - PAGE FAULT")
            # TODO: handle page fault
            page_replacement_algorithm(entry)  
            entry.auxillary = i
          
        # after handling page fault, page is valid
        # print("Valid")
        if future.access_type[i] == "W":
            entry.dirty = True
        entry.referenced = True
        # auxillary information for min algorithm
        # find next access time for this entry (that isn't the present)
        for j in range(i + 1, len(future.process_id)):
            # print(f"looking... {j}, {future.process_id[j]} - {process_id}, {future.VPN[j]} - {VPN}")
            if future.process_id[j] == process_id and future.VPN[j] == VPN:
                entry.auxillary = j
                # print("future hit")
                break
            # if j == len(future.process_id) - 1:
            #     print("fall through")

        sim_time += 1


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
    victim = min(enumerate(main_memory_buffer), key = lambda s: (-s[1].auxillary, s[0]))[0]
    # update statistics and replace page in main memory
    if main_memory_buffer[victim].dirty:
        total_disk_references += 1
        total_dirty_page_writes += 1
    main_memory_buffer[victim].clear()
    entry.translation = victim
    main_memory_buffer[victim] = entry


main()
# print(len(future.process_id), len(future.VPN))
# print(sim_time)
print("MIN", end=" ")
print(total_page_faults, total_disk_references, total_dirty_page_writes)
print("--   Done    --")