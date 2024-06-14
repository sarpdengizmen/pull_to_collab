"""
This code is used to test the two different implementations of k_means algorithm.
It creates the input file with the given parameters and runs the two different implementations of k_means algorithm.
It then compares the time taken by the two different implementations.
"""
def create_input_file(filename, num_points, num_clusters, data_dimension, tolerance, num_threads):
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
        f.write(f'{num_threads}\n')

import os
import subprocess
import time

# The directory containing the input files, which is in the same directory as this script
input_dir = './'
datasets = [("brich3_N10000.dat", 100000, 2, 1.0E-9), ("circle_N4000.dat", 4000, 2, 1.0E-9), ("hepta_N212.dat", 212, 3, 1.0E-9), ("isolation_N9000.dat", 9000, 2, 1.0E-9), ("smile_N1000.dat", 1000, 2, 1.0E-9)]

# The range of thread counts to test
thread_counts = range(1, 8)


# The number of times to run each test
num_runs = 20

# The names of your k-means implementations
programs = ['cluster', 'cluster_old']

# Open the output file
with open('output.txt', 'w') as f:
    # Write the header row
    f.write('Input file\tThread count\tImplementation\tAverage time\n')

    # For each input file
    for input_file in datasets:
        for program in programs:
            # If the program is 'cluster_old', run it only once with a single thread
            if program == 'cluster_old':
                create_input_file("input.dat", input_file[1], 7, input_file[2], input_file[3], 1)
                total_time = 0
                for i in range(num_runs):
                    start_time = time.time()
                    subprocess.run(['./' + program, "input.dat" ,input_file[0]])
                    end_time = time.time()
                    total_time += end_time - start_time
                average_time = total_time / num_runs
                f.write(f'{input_file[0]}\t1\t{program}\t{average_time}\n')
            else:
                for num_threads in thread_counts:
                    create_input_file("input.dat", input_file[1], 7, input_file[2], input_file[3], num_threads)
                    total_time = 0
                    for i in range(num_runs):
                        start_time = time.time()
                        subprocess.run(['./' + program, "input.dat" ,input_file[0]])
                        end_time = time.time()
                        total_time += end_time - start_time
                    average_time = total_time / num_runs * 1000
                    f.write(f'{input_file[0]}\t{num_threads}\t{program}\t{average_time}\n')
            


