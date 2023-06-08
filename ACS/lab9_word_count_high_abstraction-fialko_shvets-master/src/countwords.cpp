// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <boost/locale.hpp>
#include <fmt/format.h>
#include <utility>
#include <vector>
#include <filesystem>
#include <archive.h>
#include <archive_entry.h>

#include "time_measurements.hpp"
#include "countwords.hpp"

CountWords::CountWords(std::unique_ptr<countwords_config> config_ptr) :
        indir{config_ptr->indir},
        out_by_a{config_ptr->out_by_a},
        out_by_n{config_ptr->out_by_n},
        indexing_extensions{config_ptr->indexing_extensions},
        archives_extensions{config_ptr->archives_extensions},
        max_file_size{config_ptr->max_file_size},
        words_dictionary() {

    boost::locale::generator gen;
    std::locale loc = gen("en_US.UTF-8");
    std::locale::global(loc);
}

void CountWords::manager() {
    auto start = get_current_time_fenced();

    if (!std::filesystem::exists(indir)) {
        std::cerr << "Error: input directory " << indir << " does not exist." << std::endl;
        exit(26);
    }

    auto iter = std::filesystem::recursive_directory_iterator(indir);
    auto iter_end = std::filesystem::end(iter);

    oneapi::tbb::parallel_pipeline(16,
                                   oneapi::tbb::make_filter<void, std::filesystem::path>(
                                           oneapi::tbb::filter_mode::serial_out_of_order,
                                           [&](oneapi::tbb::flow_control &fc) -> std::filesystem::path {
                                               auto start = get_current_time_fenced();
                                               auto temp = find_files(fc, iter, iter_end);
                                               auto end = get_current_time_fenced();
                                               finding_time += to_ms(end - start);
                                               return temp;
                                           }) &
                                   oneapi::tbb::make_filter<std::filesystem::path, std::pair<std::string, std::string>>(
                                           oneapi::tbb::filter_mode::serial_out_of_order,
                                           [this](const std::filesystem::path &filename) -> std::pair<std::string, std::string> {
                                               auto start = get_current_time_fenced();
                                               auto temp = read_files_into_binaries(filename);
                                               auto end = get_current_time_fenced();
                                               reading_time += to_ms(end - start);
                                               return temp;
                                           }) &
                                   oneapi::tbb::make_filter<std::pair<std::string, std::string>, void>(
                                           oneapi::tbb::filter_mode::parallel,
                                           [this](const std::pair<std::string, std::string>& raw_file) {
                                               count_words(raw_file);
                                           })
    );

    auto end = get_current_time_fenced();
    total_time = to_ms(end - start);
}

std::filesystem::path
CountWords::find_files(oneapi::tbb::flow_control& fc, std::filesystem::recursive_directory_iterator &iter,
                       std::filesystem::recursive_directory_iterator &iter_end) {

    if (iter == iter_end) {
        fc.stop();
        return {};
    }

    auto entry = *(iter++);

    std::string extension = entry.path().extension();
    if ((std::find(indexing_extensions.begin(), indexing_extensions.end(), extension) != indexing_extensions.end())
        && (entry.file_size() < max_file_size) && (entry.file_size() > 0)) {
        return entry.path();
    } else if (std::find(archives_extensions.begin(), archives_extensions.end(), extension) !=
               archives_extensions.end()) {
        return entry.path();
    }

    return {};
}

std::pair<std::string, std::string> CountWords::read_files_into_binaries(const std::filesystem::path &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
//            std::cerr << "Failed to open file " << filename << std::endl;
        return {};
    }
    auto const start_pos = file.tellg();
    file.ignore(std::numeric_limits<std::streamsize>::max());
    auto const char_count = file.gcount();
    file.seekg(start_pos);
    auto s = std::string(char_count, char{});
    file.read(&s[0], static_cast<std::streamsize>(s.size()));
    file.close();

    return std::pair<std::string, std::string>{s, filename.extension()};
}

void CountWords::count_words(const std::pair<std::string, std::string>& file_content) {

    if (std::find(archives_extensions.begin(), archives_extensions.end(), file_content.second) !=
        archives_extensions.end()) {

        struct archive *archive = archive_read_new();
        if (!archive) {
//                std::cerr << "Failed to create archive object!" << std::endl;
            return;
        }

        struct archive_entry *entry;

        archive_read_support_format_all(archive);
        archive_read_support_filter_all(archive);

        int r = archive_read_open_memory(archive, file_content.first.data(), file_content.first.size());
        if (r != ARCHIVE_OK) {
//                std::cerr << "Failed to open archive!" << std::endl;
            return;
        }

        while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
            std::string extension = std::filesystem::path(archive_entry_pathname(entry)).extension().string();
            if (std::find(indexing_extensions.begin(), indexing_extensions.end(), extension) ==
                indexing_extensions.end()) {
                continue;
            }
            size_t size = archive_entry_size(entry);
            if (size > max_file_size || size <= 0) {
                continue;
            }
            std::string contents;
            contents.resize(size);
            long long k = archive_read_data(archive, &contents[0], size);
            if (k <= 0) {
                continue;
            }

            contents = boost::locale::fold_case(boost::locale::normalize(contents));
            boost::locale::boundary::ssegment_index map(boost::locale::boundary::word, contents.begin(),
                                                        contents.end());
            map.rule(boost::locale::boundary::word_letters);

            words_map temp_map;
            for (auto it = map.begin(), e = map.end(); it != e; ++it) {
                ++temp_map[*it];
            }

            merge_dictionaries(temp_map);
        }

        archive_read_free(archive);
    } else {
        std::string contents = boost::locale::fold_case(boost::locale::normalize(file_content.first));
        boost::locale::boundary::ssegment_index map(boost::locale::boundary::word, contents.begin(),
                                                    contents.end());
        map.rule(boost::locale::boundary::word_letters);

        words_map temp_map;
        for (const auto &it: map) {
            ++temp_map[it];
        }

        merge_dictionaries(temp_map);
    }
}

void CountWords::merge_dictionaries(words_map &temp_map) {
    for (auto const &[key, val]: temp_map) {
        concurrent_hash_map::accessor a;
        words_dictionary.insert(a, key);
        a->second += val;
    }
}


void CountWords::write() {
    auto start_time = get_current_time_fenced();

    std::ofstream out_by_a_file(out_by_a);
    if (!out_by_a_file.is_open()) {
        std::cerr << "Error: failed to open file for writing: " << out_by_a;
        exit(4);
    }
    // sort words by alphabet
    std::vector<std::pair<std::string, int>> alphabet_words(words_dictionary.begin(), words_dictionary.end());
    std::sort(alphabet_words.begin(), alphabet_words.end(),
              [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
                  return a.first < b.first;
              });

    try {
        for (auto const &[key, val]: alphabet_words) {
            out_by_a_file << fmt::format("{:20} {}", key, val) << std::endl;
        }
    } catch (std::exception &e) {
        std::cerr << "Error writing in an out file: " << out_by_a;
        exit(6);
    }

    out_by_a_file.close();

    std::ofstream out_by_n_file(out_by_n);
    if (!out_by_n_file.is_open()) {
        std::cerr << "Error: failed to open file for writing: " << out_by_n;
        exit(4);
    }
    // sort by number
    std::vector<std::pair<std::string, int>> number_words(words_dictionary.begin(), words_dictionary.end());
    std::sort(number_words.begin(), number_words.end(),
              [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
                  return a.second > b.second || (a.second == b.second && a.first < b.first);
              });

    try {
        for (auto const &[key, val]: number_words) {
            out_by_n_file << fmt::format("{:20} {}", key, val) << std::endl;
        }
    } catch (std::exception &e) {
        std::cerr << "Error writing in an out file: " << out_by_n;
        exit(6);
    }

    out_by_n_file.close();

    auto end_time = get_current_time_fenced();
    writing_time = to_ms(end_time - start_time);
}

void CountWords::print_time() const {
    std::cout << "Total=" << total_time << std::endl;
    std::cout << "Finding=" << finding_time << std::endl;
    std::cout << "Reading=" << reading_time << std::endl;
    std::cout << "Writing=" << writing_time << std::endl;
}