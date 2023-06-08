// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef COUNTWORDS_SEQ_COUNTWORDS_HPP
#define COUNTWORDS_SEQ_COUNTWORDS_HPP

#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include "countwords_config.hpp"
#include "ts_queue.h"

class CountWords {
    std::string indir, out_by_a, out_by_n;
    std::vector<std::string> indexing_extensions, archives_extensions;
    size_t max_file_size, indexing_threads, merging_threads;
    size_t filenames_queue_size, raw_files_queue_size, dictionaries_queue_size;

    ts_queue<std::filesystem::path> filenames_queue;
    ts_queue<std::pair<std::string, std::string>> raw_files_queue;
    ts_queue<std::unordered_map<std::string, int>> dictionaries_queue;

    std::unordered_map<std::string, int> words;

    long long total_time, finding_time, reading_time, writing_time;
public:
    explicit CountWords(std::unique_ptr<countwords_config> config_ptr);

    void find_files();
    void read_files_into_binaries();
    void count_words();
    void merge_dictionaries();

    void manager();
    void write();

    void print_time();
};

#endif //COUNTWORDS_SEQ_COUNTWORDS_HPP
