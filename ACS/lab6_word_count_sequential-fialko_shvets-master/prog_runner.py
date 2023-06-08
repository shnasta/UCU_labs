import subprocess
import sys
import statistics

# Parse command line arguments
if len(sys.argv) != 2:
    print("Usage: python prog_runner.py <num_executions>")
    sys.exit(1)
else:
    try:
        num_executions = int(sys.argv[1])
    except Exception as e:
        print("Error <num_executions> must be int")
        sys.exit(2)


# Run the C++ program with the given arguments
results = []

path = "./bin/countwords_seq"

for i in range(num_executions):
    cmd = f"{path} ./config_file.cfg"
    output = subprocess.check_output(cmd.split()).decode().strip().split("\n")
    total = int(output[0][6:])
    results.append(total)

# Check if the results are equal for all runs

# Compute statistics for each method
print("min total: ", min(results))
print("avg total: ", sum(results) / num_executions)
print("std dev: ", statistics.stdev(results) if num_executions > 1 else 0)
