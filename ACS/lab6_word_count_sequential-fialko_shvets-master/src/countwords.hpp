#ifndef COUNTWORDS_SEQ_COUNTWORDS_HPP
#define COUNTWORDS_SEQ_COUNTWORDS_HPP

#include <memory>
#include <vector>
#include <unordered_map>
#include "countwords_config.hpp"

class CountWords {
    std::string indir, out_by_a, out_by_n;
    std::vector<std::string> indexing_extensions, archives_extensions;
    size_t max_file_size;

    std::unordered_map<std::string, int> words;
public:
    explicit CountWords(std::unique_ptr<countwords_config> config_ptr);

    long long manager();
    void count_words(std::string& file_content);
    void read_archive(const std::string& filename);
    long long write();

    static std::string read_file_into_string(const std::string& filename);
};

#endif //COUNTWORDS_SEQ_COUNTWORDS_HPP
