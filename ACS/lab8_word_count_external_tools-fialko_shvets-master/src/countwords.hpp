// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef COUNTWORDS_SEQ_COUNTWORDS_HPP
#define COUNTWORDS_SEQ_COUNTWORDS_HPP

#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <mutex>
#include "countwords_config.hpp"
#include "oneapi/tbb/concurrent_hash_map.h"
#include "oneapi/tbb/concurrent_queue.h"

class CountWords {
    using words_map = std::unordered_map<std::string, int>;
    template<class T>
    using concurrent_queue = oneapi::tbb::concurrent_bounded_queue<T>;
    using concurrent_hash_map = oneapi::tbb::concurrent_hash_map<std::string, int>;

    std::string indir, out_by_a, out_by_n;
    std::vector<std::string> indexing_extensions, archives_extensions;
    size_t max_file_size, indexing_threads, merging_threads;
    size_t filenames_queue_size, raw_files_queue_size, dictionaries_queue_size;

    concurrent_queue<std::filesystem::path> filenames_queue;
    concurrent_queue<std::pair<std::string, std::string>> raw_files_queue;
    concurrent_queue<words_map> dictionaries_queue;
    concurrent_hash_map words_dictionary;


    size_t remaining_poison_pills{};
    std::mutex merge_mutex;

    long long total_time{}, finding_time{}, reading_time{}, writing_time{};
public:
    explicit CountWords(std::unique_ptr<countwords_config> config_ptr);

    void find_files();
    void read_files_into_binaries();
    void count_words();
    void merge_dictionaries();

    void manager();
    void write();

    void print_time() const;
};

#endif //COUNTWORDS_SEQ_COUNTWORDS_HPP
