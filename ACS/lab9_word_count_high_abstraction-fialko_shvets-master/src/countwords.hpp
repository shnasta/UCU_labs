// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef COUNTWORDS_SEQ_COUNTWORDS_HPP
#define COUNTWORDS_SEQ_COUNTWORDS_HPP

#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include "countwords_config.hpp"
#include "oneapi/tbb/concurrent_hash_map.h"
#include "oneapi/tbb/concurrent_queue.h"
#include "oneapi/tbb/parallel_pipeline.h"

class CountWords {
    using words_map = std::unordered_map<std::string, int>;
    using concurrent_hash_map = oneapi::tbb::concurrent_hash_map<std::string, int>;

    std::string indir, out_by_a, out_by_n;
    std::vector<std::string> indexing_extensions, archives_extensions;
    size_t max_file_size;

    concurrent_hash_map words_dictionary;

    long long total_time{}, finding_time{}, reading_time{}, writing_time{};
public:
    explicit CountWords(std::unique_ptr<countwords_config> config_ptr);

    std::filesystem::path find_files(oneapi::tbb::flow_control& fc, std::filesystem::recursive_directory_iterator& it,
                                     std::filesystem::recursive_directory_iterator& end);
    std::pair<std::string, std::string> read_files_into_binaries(const std::filesystem::path& filename);
    void count_words(const std::pair<std::string, std::string>& raw_file);
    void merge_dictionaries(words_map& dictionary);

    void manager();
    void write();

    void print_time() const;
};

#endif //COUNTWORDS_SEQ_COUNTWORDS_HPP
