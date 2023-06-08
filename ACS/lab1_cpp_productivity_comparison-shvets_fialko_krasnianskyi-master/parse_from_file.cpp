// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include "options_parser.h"
#include "read_from_file.h"

#include <chrono>
#include <atomic>

//#define DEBUG

inline std::chrono::high_resolution_clock::time_point get_current_time_fenced()
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<typename D>
inline long long to_ms(const D& d)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
}

int main(int argc, char* argv[]) {
    command_line_options_t command_line_options{argc, argv};

    std::vector<std::string> filenames = command_line_options.get_filenames();

    auto start_time = get_current_time_fenced();
    std::vector<std::string> result = read_from_file(filenames[0], filenames[1]);
    auto finish_time = get_current_time_fenced();
    auto total_time = finish_time - start_time;

#ifdef DEBUG
    int i = 0;
    for (auto &str : result) {
        std::cout << str << "  " << i << std::endl;
        i += 1;
    }
#endif

    int sum = 0;
    for (auto &str : result) {
        sum += str.length();
    }

    std::cout << to_ms(total_time) << std::endl;
    std::cout << sum << std::endl;
    return 0;
}
