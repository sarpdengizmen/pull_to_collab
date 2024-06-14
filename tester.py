"""
This code is used to test the two different implementations of k_means algorithm.
It creates the input file with the given parameters and runs the two different implementations of k_means algorithm.
It then compares the time taken by the two different implementations.
"""
def create_input_file(filename, num_points, num_clusters, data_dimension, tolerance):
    with open(filename, 'w') as f:
        f.write('[NUMBER_OF_POINTS]\n')
        f.write(f'{num_points}\n')
        f.write('[NUMBER_OF_CLUSTERS]\n')
        f.write(f'{num_clusters}\n')
        f.write('[DATA_DIMENSION]\n')
        f.write(f'{data_dimension}\n')
        f.write('[TOLERANCE]\n')
        f.write(f'{tolerance}\n')
        f.write('[NUMBER_OF_THREADS]\n')
        f.write(f'8\n')


import os
import subprocess
import time

# The directory containing the input files, which is in the same directory as this script
input_dir = './'
datasets = [("brich3_N10000.dat", 100000, 2, 1.0E-4)]

# The range of thread counts to test
datasizes = range(1, 21)


# The number of times to run each test
num_runs = 40

# The names of your k-means implementations
programs = ['kmeans_serial', 'kmeans_omp', 'kmeans_gpu']

# Open the output file
with open('output.txt', 'w') as f:
    # Write the header row
    f.write('Input file\tThread count\tImplementation\tAverage time\tSpeedUp (Sp)\n')

    # For each input file
    for input_file in datasets:
        for program in programs:
            for multiplier in datasizes:
                create_input_file("input.dat", multiplier*1000, 4, input_file[2], input_file[3])
                total_time = 0
                for i in range(num_runs):
                    result = subprocess.run(['./' + program, "input.dat" ,input_file[0]], capture_output=True, text=True)
                    last_line = result.stdout.strip().split('\n')[-1]
                    # find : in the last line
                    colon_index = last_line.find(':')
                    last_line = last_line[colon_index+1:]
                    algorithm_time = float(last_line)
                    print(f'Ran {program} with 8 threads, run {i+1} of {num_runs}, time: {algorithm_time} seconds')
                    total_time += algorithm_time
                average_time = total_time / num_runs 
                f.write(f'{input_file[0]}\t{program}\t{average_time}\t\n')
            


