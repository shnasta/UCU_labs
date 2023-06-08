import subprocess
import sys
import numpy as np
import matplotlib.pyplot as plt

from prog_runner import (
    setting_args,
    read_results,
    assert_words_equal,
    flush_cache
)

config_file = "./config_file.cfg"


def get_serial_time(program_path):
    result = subprocess.run(
        [program_path, config_file], capture_output=True
    )
    if result.returncode != 0:
        raise ValueError(
            "Something went wrong. Error code " + str(result.returncode)
        )

    output = result.stdout.decode("utf-8").split("\n")

    return int(output[0].split("=")[1])


def main():
    # Define the path to the C++ program
    path = "bin/countwords_par"

    # Define the file to read the results from
    results_file = "res_a.txt"

    # Set the command line arguments
    iterations, clear_cache = setting_args(sys.argv)

    threads = [1, 2, 4, 8, 16]  # 32, 64, 128]

    T1 = 0
    times_for_merge = []
    performance = []

    for num_merge_threads in threads:

        # Set the number of threads for merging in the config file
        with open(config_file, "r+") as f:
            lines = f.readlines()
            lines[7] = f"merging_threads= {num_merge_threads}\n"  # add a newline character at the end
            f.seek(0)  # move the file pointer to the beginning of the file
            f.writelines(lines)  # write the modified lines back to the file
            f.truncate()

        times_for_index = []
        eff_for_index = []

        for num_index_threads in threads:
            # Set the number of threads for indexing in the config file
            with open(config_file, "r+") as f:
                lines = f.readlines()
                lines[6] = f"indexing_threads= {num_index_threads}\n"  # add a newline character at the end
                f.seek(0)  # move the file pointer to the beginning of the file
                f.writelines(lines)  # write the modified lines back to the file
                f.truncate()

            words_count = None
            total_times = []

            for _ in range(iterations):
                flush_cache(clear_cache)

                result = subprocess.run(
                    [path, config_file], capture_output=True
                )
                if result.returncode != 0:
                    raise ValueError(
                        "Something went wrong. Error code " + str(result.returncode)
                    )

                output = result.stdout.decode("utf-8").split("\n")

                words_count = assert_words_equal(words_count, read_results(results_file))

                total_times.append(int(output[0].split("=")[1]))

            # calculate efficiency for indexing with current number of threads
            times_for_index.append(min(total_times))

            if num_index_threads == 1 and num_merge_threads == 1:
                T1 = min(total_times)

            print(num_merge_threads, num_index_threads)
            print(T1)
            print("Added efficiency: ", T1 / min(total_times) / (num_index_threads * num_merge_threads))

            eff_for_index.append(T1 / min(total_times) / (num_index_threads * num_merge_threads))

        times_for_merge.append(times_for_index)
        performance.append(eff_for_index)

    # Plot the results
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    X, Y = np.meshgrid(threads, threads)
    Z = np.array(performance)

    ax.plot_surface(X, Y, Z)

    ax.set_xlabel('Merging threads')
    ax.set_ylabel('Indexing threads')
    ax.set_zlabel('Efficiency')
    plt.savefig("res_plot.png")
    plt.show()


if __name__ == "__main__":
    main()
