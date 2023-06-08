import subprocess
import sys
import os
import platform
import csv
import statistics
from scipy.stats import wilcoxon


# Function to flush OS buffers
def do_flush_buffers():
    os_type = platform.system()
    if os_type == "Windows":
        os.system("pause")
        os.system("cls")
    elif os_type == "Linux":
        os.system("sync")
    elif os_type == "Darwin":
        os.system("sync")
        os.system("sudo -S /usr/sbin/purge")
    else:
        print("Unsupported OS type")
        sys.exit(123456789)


# Parse command line arguments
if len(sys.argv) != 5:
    print("Usage: python prog1_runner.py <text_file> <num_executions> <flush_buffers> <output_file>")
    sys.exit(1)
else:
    text_file = sys.argv[1]
    num_executions = int(sys.argv[2])
    flush_buffers = bool(int(sys.argv[3]))
    output_file = sys.argv[4]

if flush_buffers:
    do_flush_buffers()

# Run the C++ program with the given arguments
results = {}

if not os.path.exists(text_file):
    print(f"Error: File {text_file} does not exist")
    sys.exit(3)

path = "./bin/performance_strings"
# if file does not exist, run ./compile.sh
if not os.path.exists(path):
    subprocess.check_output("./compile.sh".split())


for method in range(1, 6):
    method_results = []
    for i in range(num_executions):
        cmd = f"{path} {method} {text_file}"
        if flush_buffers:
            do_flush_buffers()
        output = subprocess.check_output(cmd.split()).decode().strip().split("\n")
        time = int(output[0])
        result = output[1]
        method_results.append((time, result))
    results[method] = method_results

# Check if the results are equal for all runs
for method in results:
    first_result = results[method][0][1]
    for i in range(1, num_executions):
        if first_result != results[method][i][1]:
            print(f"Error: Results for method {method} are not consistent")
            sys.exit()

# Compute statistics for each method
stats = {}
for method in results:
    times = [result[0] for result in results[method]]
    stats[method] = {
        "min_time": min(times),
        "avg_time": sum(times) / num_executions,
        "std_dev": statistics.stdev(times) if num_executions > 1 else 0
    }
    print(f"Method {method}:")
    print(f"Min execution time: {stats[method]['min_time']} ms")
    print(f"Avg execution time: {stats[method]['avg_time']} ms")
    print(f"Corrected sample standard deviation: {stats[method]['std_dev']:.2f} ms")

# Compute p-values and save to CSV file
with open(output_file, "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(["Method 1 vs Method 2", "Method 1 vs Method 3", "Method 1 vs Method 4", "Method 1 vs Method 5",
                     "Method 2 vs Method 3", "Method 2 vs Method 4", "Method 2 vs Method 5", "Method 3 vs Method 4",
                     "Method 3 vs Method 5", "Method 4 vs Method 5"])
    p_values = []
    try:
        for i in range(5):
            for j in range(i + 1, 5):
                x = [result[i][0] for result in results.values()]
                y = [result[j][0] for result in results.values()]
                _, p_value = wilcoxon(x, y)
                p_values.append(p_value)
        writer.writerow(p_values)
    except Exception as e:
        sys.exit(6)

