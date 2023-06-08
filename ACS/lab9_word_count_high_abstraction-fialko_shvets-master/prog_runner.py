import os
import subprocess
import sys
from sys import platform
import statistics


# Define a function to set the command line arguments
def setting_args(args):
    if len(args) != 3:
        print("Usage: python prog_runner.py <num_executions> <clear_cache>")
        sys.exit(1)
    else:
        try:
            num_executions = int(sys.argv[1])
        except ValueError:
            print("Error <num_executions> must be int")
            sys.exit(2)
        try:
            clear_cache = int(sys.argv[2])
            if clear_cache != 0 and clear_cache != 1:
                print("Error <clear_cache> must be 0 or 1")
                sys.exit(2)
        except ValueError:
            print("Error <clear_cache> must be int")
            sys.exit(2)
    return num_executions, clear_cache


# Parse command line arguments

# Define a function to read the results from the file
def read_results(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()

    word_counts = {}

    for line in lines:
        word, count = line.strip().split()
        word_counts[word] = int(count)

    return word_counts


# Define a function to print the times
def print_times(times):
    print(min(times))
    print(sum(times) / len(times))
    print(statistics.stdev(times) if len(times) > 1 else 0)
    print()


def assert_words_equal(previous_word_counts, current_word_counts):
    # Check if the results are the same as the previous iteration
    if previous_word_counts is None:
        return current_word_counts
    elif previous_word_counts != current_word_counts:
        print("Error: results are not equal")
        sys.exit(3)
    return current_word_counts


def flush_cache(flush):
    if flush:
        system = detect_system()
        if system == "Linux":
            os.system("sync")
            os.system("sh -c 'echo 3 > /proc/sys/vm/drop_caches'")
        elif system == "MacOS":
            os.system("purge")
        else:
            # raise ValueError("Unsupported system")
            raise ValueError("Stop. Get some help.")


def detect_system():
    os_enum = {
        "linux": "Linux",
        "linux2": "Linux",
        "win32": "Windows",
        "darwin": "MacOS"
    }
    return os_enum[platform]


def main():
    # Set the command line arguments
    num_executions, clear_cache = setting_args(sys.argv)

    # Define the path to the C++ program and the config file
    path = "bin/countwords_par_tbb"
    config_file = "./config_file.cfg"

    # Define the file to read the results from
    results_file = "res_a.txt"

    # Run the C++ program with the given arguments and measure the execution time
    total_times = []
    finding_times = []
    reading_times = []
    writing_times = []

    # Keep track of the word counts from each iteration
    previous_word_counts = None

    for i in range(num_executions):
        # Flush the cache
        flush_cache(clear_cache)

        # Run the program
        result = subprocess.run([path, config_file], capture_output=True)
        if result.returncode != 0:
            raise ValueError(
                "Something went wrong. Error code " + str(result.returncode)
            )

        output = result.stdout.decode("utf-8").split("\n")

        assert_words_equal(previous_word_counts, read_results(results_file))

        total_times.append(int(output[0].split("=")[1]))
        finding_times.append(int(output[1].split("=")[1]))
        reading_times.append(int(output[2].split("=")[1]))
        writing_times.append(int(output[3].split("=")[1]))

    # Print the results
    print_times(total_times)
    print_times(finding_times)
    print_times(reading_times)
    print_times(writing_times)


if __name__ == "__main__":
    main()
