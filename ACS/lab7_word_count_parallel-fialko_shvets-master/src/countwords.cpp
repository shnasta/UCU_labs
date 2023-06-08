// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <boost/locale.hpp>
#include <fmt/format.h>
#include <vector>
#include <filesystem>
#include <thread>
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
        filenames_queue_size{config_ptr->filenames_queue_size},
        raw_files_queue_size{config_ptr->raw_files_queue_size},
        dictionaries_queue_size{config_ptr->dictionaries_queue_size},
        indexing_threads{config_ptr->indexing_threads},
        merging_threads{config_ptr->merging_threads},
        filenames_queue(filenames_queue_size),
        raw_files_queue(raw_files_queue_size),
        dictionaries_queue(dictionaries_queue_size) {

    boost::locale::generator gen;
    std::locale loc = gen("en_US.UTF-8");
    std::locale::global(loc);
}

void CountWords::manager() {
    auto start = get_current_time_fenced();

    dictionaries_queue.set_active_index_threads(indexing_threads);

    std::vector<std::thread> threads;
    threads.emplace_back(&CountWords::find_files, this);
    threads.emplace_back(&CountWords::read_files_into_binaries, this);
    for (size_t i = 0; i < indexing_threads; ++i) {
        threads.emplace_back(&CountWords::count_words, this);
    }
    for (size_t i = 0; i < merging_threads; ++i) {
        threads.emplace_back(&CountWords::merge_dictionaries, this);
    }
    for (auto &thread: threads) {
        thread.join();
    }

    auto end = get_current_time_fenced();
    total_time = to_ms(end - start);
}

void CountWords::find_files() {
    auto start = get_current_time_fenced();

    if (!std::filesystem::exists(indir)) {
        std::cerr << "Error: input directory " << indir << " does not exist." << std::endl;
        exit(26);
    }

    for (const auto &entry: std::filesystem::recursive_directory_iterator(indir)) {
        std::string extension = entry.path().extension().string();
        if ((std::find(indexing_extensions.begin(), indexing_extensions.end(), extension) != indexing_extensions.end())
            && (entry.file_size() < max_file_size) && (entry.file_size() > 0)) {
            filenames_queue.push(const_cast<std::filesystem::path &&>(entry.path()));
        } else if (std::find(archives_extensions.begin(), archives_extensions.end(), extension) !=
                   archives_extensions.end()) {
            filenames_queue.push(const_cast<std::filesystem::path &&>(entry.path()));
        }
    }
    filenames_queue.push(std::filesystem::path{}); // end of queue

    auto end = get_current_time_fenced();
    finding_time = to_ms(end - start);
}

void CountWords::read_files_into_binaries() {
    auto start = get_current_time_fenced();

    while (true) {
        std::filesystem::path filename = filenames_queue.pop();
        if (filename.empty()) {
            for (size_t i = 0; i < indexing_threads; ++i) {
                raw_files_queue.push(std::move(std::pair<std::string, std::string>{})); // end of queue
            }
            break;
        }
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
//            std::cerr << "Failed to open file " << filename << std::endl;
            continue;
        }
        auto const start_pos = file.tellg();
        file.ignore(std::numeric_limits<std::streamsize>::max());
        auto const char_count = file.gcount();
        file.seekg(start_pos);
        auto s = std::string(char_count, char{});
        file.read(&s[0], static_cast<std::streamsize>(s.size()));
        file.close();

        if (!s.empty()) {
            raw_files_queue.push(std::move(std::pair<std::string, std::string>{s, filename.extension().string()}));
        }
    }

    auto end = get_current_time_fenced();
    reading_time = to_ms(end - start);
}

void CountWords::count_words() {
    while (true) {
        std::pair<std::string, std::string> file_content = raw_files_queue.pop();
        if (file_content.first.empty() || file_content.second.empty()) {
            break;
        }

        if (std::find(archives_extensions.begin(), archives_extensions.end(), file_content.second) !=
            archives_extensions.end()) {

            struct archive *archive = archive_read_new();
            if (!archive) {
//                std::cerr << "Failed to create archive object!" << std::endl;
                continue;
            }
            struct archive_entry *entry;

            archive_read_support_format_all(archive);
            archive_read_support_filter_all(archive);

            int r = archive_read_open_memory(archive, file_content.first.data(), file_content.first.size());
            if (r != ARCHIVE_OK) {
//                std::cerr << "Failed to open archive!" << std::endl;
                continue;
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


                std::unordered_map<std::string, int> current_words;

                contents = boost::locale::fold_case(boost::locale::normalize(contents));
                boost::locale::boundary::ssegment_index map(boost::locale::boundary::word, contents.begin(),
                                                            contents.end());
                map.rule(boost::locale::boundary::word_letters);

                for (boost::locale::boundary::ssegment_index::iterator it = map.begin(), e = map.end(); it != e; ++it) {
                    ++current_words[*it];
                }

                if (!current_words.empty()) {
                    dictionaries_queue.push(std::move(current_words));
                }
            }

            archive_read_free(archive);
        } else {
            std::unordered_map<std::string, int> current_words;

            file_content.first = boost::locale::fold_case(boost::locale::normalize(file_content.first));
            boost::locale::boundary::ssegment_index map(boost::locale::boundary::word, file_content.first.begin(),
                                                        file_content.first.end());
            map.rule(boost::locale::boundary::word_letters);

            for (boost::locale::boundary::ssegment_index::iterator it = map.begin(), e = map.end(); it != e; ++it) {
                ++current_words[*it];
            }

            if (!current_words.empty()) {
                dictionaries_queue.push(std::move(current_words));
            }
        }
    }
    dictionaries_queue.subtract_active_index_threads();
}

void CountWords::merge_dictionaries() {
    while (true) {
        auto map_pair = dictionaries_queue.pop_pair();

        if (map_pair.second.empty()) {
            if (!map_pair.first.empty()) {
                words = std::move(map_pair.first);
            }
            break;
        }

        dictionaries_queue.inc_active_merge_threads();
        if (map_pair.first.size() > map_pair.second.size()) {
            for (auto const &[key, val]: map_pair.second) {
                map_pair.first[key] += val;
            }
            dictionaries_queue.push(std::move(map_pair.first));
        } else {
            for (auto const &[key, val]: map_pair.first) {
                map_pair.second[key] += val;
            }
            dictionaries_queue.push(std::move(map_pair.second));
        }

        dictionaries_queue.subtract_active_merge_threads();
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
    std::vector<std::pair<std::string, int>> alphabet_words(words.begin(), words.end());
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
    std::vector<std::pair<std::string, int>> number_words(words.begin(), words.end());
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

void CountWords::print_time() {
    std::cout << "Total=" << total_time << std::endl;
    std::cout << "Finding=" << finding_time << std::endl;
    std::cout << "Reading=" << reading_time << std::endl;
    std::cout << "Writing=" << writing_time << std::endl;
}