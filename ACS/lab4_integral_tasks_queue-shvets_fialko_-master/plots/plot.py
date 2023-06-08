import matplotlib.pyplot as plt
import subprocess


results = {}

path = "../bin/integrate_parallel_queue"
num_threads = [1, 2, 4, 8, 16, 100]
num_executions = 1
points_per_task = 500

for method in range(1, 4):
    method_times = []
    for num_of_threads in num_threads:
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

        p = 10 ** (-7)
        first_result = results[method][0][1]
        for i in range(1, num_executions):
            if abs(first_result - results[method][i][1]) > p:
                print(f"Error: Results for method {method} are not consistent")
                sys.exit()
        print(f"method {method} with {num_of_threads} threads: {results[method][0][0]}   DONE")
        method_times.append(min([result[0] for result in results[method]]))

    L1 = method_times[0]
    res = [L1/method_times[i]/num_threads[i] for i in range(len(method_times))]

    plt.xlabel("Number of threads")
    plt.ylabel("Efficiency")
    plt.title(f"Efficiency for function {method}")
    plt.plot(num_threads, res, 'ro')
    plt.savefig(f"res_plot{method}.png")
    plt.show()
