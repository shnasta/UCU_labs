import subprocess
import sys
import statistics

# Parse command line arguments
if len(sys.argv) != 4:
    print("Usage: python prog_runner.py <num_executions> <num_of_threads> <points_per_task>")
    sys.exit(1)
else:
    try:
        num_executions = int(sys.argv[1])
        num_of_threads = int(sys.argv[2])
        points_per_task = int(sys.argv[3])
    except Exception as e:
        print("Error <num_executions> must be int")
        sys.exit(2)


# Run the C++ program with the given arguments
results = {}

path = "./bin/integrate_parallel_tpool"

for method in range(1, 4):
    method_results = []
    for i in range(num_executions):
        cmd = f"{path} {method} ./func{method}.cfg {num_of_threads} {points_per_task}"
        output = subprocess.check_output(cmd.split()).decode().strip().split("\n")
        result = float(output[0])
        abs_err = float(output[1])
        rel_err = float(output[2])
        time = int(output[3])
        method_results.append((time, result, abs_err, rel_err))
        results[method] = method_results

# Check if the results are equal for all runs
p = 10 ** (-7)
for method in results:
    first_result = results[method][0][1]
    for i in range(1, num_executions):
        if abs(first_result - results[method][i][1]) > p:
            print(f"Error: Results for method {method} are not consistent")
            sys.exit()

# Compute statistics for each method
stats = {}
for method in results:
    times = [result[0] for result in results[method]]
    resultss = [result[1] for result in results[method]]
    abs_errs = [result[2] for result in results[method]]
    rel_errs = [result[3] for result in results[method]]
    stats[method] = {
        "result": min(resultss),
        "abs_err": min(abs_errs),
        "rel_err": min(rel_errs),
        "min_time": min(times),
        "avg_time": sum(times) / num_executions,
        "std_dev": statistics.stdev(times) if num_executions > 1 else 0
    }
    print(f"Function {method}:")
    print(f"Result of integration: {stats[method]['result']}")
    print(f"Absolute error: {stats[method]['abs_err']}")
    print(f"Relative error: {stats[method]['rel_err']}")
    print(f"Min execution time: {stats[method]['min_time']} ms")
    print(f"Avg execution time: {stats[method]['avg_time']} ms")
    print(f"Corrected sample standard deviation: {stats[method]['std_dev']:.2f} ms")
    print()
