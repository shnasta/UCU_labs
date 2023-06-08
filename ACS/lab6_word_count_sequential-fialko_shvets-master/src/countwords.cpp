#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <boost/locale.hpp>
#include <fmt/format.h>
#include <vector>
#include <filesystem>
#include <archive.h>
#include <archive_entry.h>

#include "time_measurements.hpp"
#include "countwords.hpp"

CountWords::CountWords(std::unique_ptr<countwords_config> config_ptr) {
    indir = config_ptr->indir;
    out_by_a = config_ptr->out_by_a;
    out_by_n = config_ptr->out_by_n;
    indexing_extensions = config_ptr->indexing_extensions;
    archives_extensions = config_ptr->archives_extensions;
    max_file_size = config_ptr->max_file_size;

    boost::locale::generator gen;
    std::locale loc = gen("en_US.UTF-8");
    std::locale::global(loc);
}

long long CountWords::manager() {
    auto start_time = get_current_time_fenced();

    if (!std::filesystem::exists(indir)) {
        std::cerr << "Error: input directory " << indir << " does not exist." << std::endl;
        exit(26);
    }

    for (const auto & entry : std::filesystem::recursive_directory_iterator(indir)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().string();
            std::string extension = entry.path().extension().string();
            if ((std::find(indexing_extensions.begin(), indexing_extensions.end(), extension) != indexing_extensions.end())
                && (entry.file_size() < max_file_size)) {
                std::string file_content = read_file_into_string(filename);
                count_words(file_content);
            } else if (std::find(archives_extensions.begin(), archives_extensions.end(), extension) != archives_extensions.end()) {
                read_archive(filename);
            }
        }
    }

    auto end_time = get_current_time_fenced();

    return to_ms(end_time - start_time);
}

void CountWords::read_archive(const std::string& filename) {
    std::string file_str = read_file_into_string(filename);

    struct archive *archive = archive_read_new();
    if (!archive) {
        std::cerr << "Failed to create archive object!" << std::endl;
    }
    struct archive_entry *entry;

    archive_read_support_format_all(archive);
    archive_read_support_filter_all(archive);

    int r = archive_read_open_memory(archive,file_str.data(),file_str.size());
    if (r != ARCHIVE_OK) {
        std::cerr << "Failed to open archive!" << std::endl;
    }

    while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
        std::string extension = std::filesystem::path(archive_entry_pathname(entry)).extension().string();
        if (std::find(indexing_extensions.begin(), indexing_extensions.end(), extension) == indexing_extensions.end()) {
            continue;
        }
        size_t size = archive_entry_size(entry);
        if (size > max_file_size) {
            continue;
        }
        std::string contents;
        contents.resize(size);
        archive_read_data(archive, &contents[0], size);
        count_words(contents);
    }

    r = archive_read_free(archive);
    if (r != ARCHIVE_OK) {
        std::cerr << "Failed to free archive!" << std::endl;
    }
}

void CountWords::count_words(std::string& file_content) {
    file_content = boost::locale::fold_case(boost::locale::normalize(file_content));

    boost::locale::boundary::ssegment_index map(boost::locale::boundary::word, file_content.begin(), file_content.end());
    map.rule(boost::locale::boundary::word_letters);

    for( boost::locale::boundary::ssegment_index::iterator it=map.begin(),e=map.end();it!=e;++it) {
        ++words[*it];
    }

}

long long CountWords::write() {
    auto start_time = get_current_time_fenced();

    std::ofstream out_by_a_file(out_by_a);
    if (!out_by_a_file.is_open()) {
        std::cerr << "Error: failed to open file for writing: " << out_by_a ;
        exit(4);
    }
    std::ofstream out_by_n_file(out_by_n);
    if (!out_by_n_file.is_open()) {
        std::cerr << "Error: failed to open file for writing: " << out_by_n ;
        exit(4);
    }

    // sort words by alphabet
    std::vector<std::pair<std::string, int>> alphabet_words(words.begin(), words.end());
    std::sort(alphabet_words.begin(), alphabet_words.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.first < b.first;
              });

    try {
        for (auto const& [key, val] : alphabet_words) {
            out_by_a_file << fmt::format("{:20} {}", key, val) << std::endl;
        }
    } catch (std::exception& e){
        std::cerr << "Error writing in an out file: " << out_by_a;
        exit(6);
    }

    // sort by number
    std::vector<std::pair<std::string, int>> number_words(words.begin(), words.end());
    std::sort(number_words.begin(), number_words.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.second > b.second;
              });

    try {
        for (auto const& [key, val] : number_words) {
            out_by_n_file << fmt::format("{:20} {}", key, val) << std::endl;
        }
    } catch (std::exception& e){
        std::cerr << "Error writing in an out file: " << out_by_n;
        exit(6);
    }

    auto end_time = get_current_time_fenced();

    return to_ms(end_time - start_time);
}

std::string CountWords::read_file_into_string(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << std::endl;
    }
    auto const start_pos = file.tellg();
    file.ignore(std::numeric_limits<std::streamsize>::max());
    auto const char_count = file.gcount();
    file.seekg(start_pos);
    auto s = std::string(char_count, char{});
    file.read(&s[0], static_cast<std::streamsize>(s.size()));
    return s;
}