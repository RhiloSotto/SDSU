To run the simulator, use the command line:
python3 page_replacement.py [input_file.txt] [RAND, FIFO, LRU, PER, MIN] [# Trials]

Results of ran simulation will be printed to the terminal and stored within the "results.csv" file

Examples to get the outputs included in the report:
- python3 page_replacement.py data1.txt RAND 100
- python3 page_replacement.py data1.txt FIFO
- python3 page_replacement.py data1.txt LRU
- python3 page_replacement.py data1.txt PER
- python3 page_replacement.py data1.txt MIN
- python3 page_replacement.py data2.txt RAND 100
- python3 page_replacement.py data2.txt FIFO
- python3 page_replacement.py data2.txt LRU
- python3 page_replacement.py data2.txt PER
- python3 page_replacement.py data2.txt MIN