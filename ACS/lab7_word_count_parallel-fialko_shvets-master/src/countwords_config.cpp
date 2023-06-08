// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "countwords_config.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>


auto countwords_config::parse_string(std::string arg) {
    constexpr char separator = '=';
    constexpr char commenter = '#';

    auto comment_pos = arg.find(commenter);
    if(comment_pos != std::string::npos)
        arg.erase(comment_pos);
    auto sep_pos = arg.find(separator);
    if(sep_pos == std::string::npos){
        boost::algorithm::trim(arg);
        return parse_string_ret_t{arg, std::nullopt};
    }
    auto left_part = arg.substr(0, sep_pos);
    auto right_part = arg.substr(sep_pos+1, std::string::npos );
    boost::algorithm::trim(left_part);
    boost::algorithm::trim(right_part);
//    check if first and last char is " or '
    if( ((right_part.front() == '"' && right_part.back() == '"')) && right_part.size() > 1) {
        right_part.erase(0, 1);
        right_part.pop_back();
    } else if (right_part.front() == '"' || right_part.back() == '"') {
        std::cerr << "Error: Wrong config line: " << arg << std::endl;
        exit(5);
    }

    return parse_string_ret_t{left_part, right_part};
}



void countwords_config::file_to_options_map(std::istream& cf) {
    std::string line;
    while (std::getline(cf, line)) {
        auto pr = parse_string(line);
        if (pr.first.empty()) {
            if (!pr.second)
                continue;
            else {
                std::cerr << "Error: Wrong config line -- no option name: " << line << std::endl;
                exit(5);
            }
        } else if (!pr.second) {
            std::cerr << "Error: Wrong config line -- no '=': " << line << std::endl;
            exit(5);
        } else if (pr.second->empty()) {
            std::cerr << "Error: Wrong config option value: " << line << std::endl;
            exit(5);
        }
        if (options_map.count(pr.first)) {
            std::cerr << "Error: Duplicated option name: " << pr.first << " = "
                      << *pr.second << "; prev. val: " << options_map[pr.first] << std::endl;
            exit(5);
        }

        if (pr.first == "indexing_extensions") {
            indexing_extensions.emplace_back(*pr.second);
        } else if (pr.first == "archives_extensions") {
            archives_extensions.emplace_back(*pr.second);
        } else {
            options_map[pr.first] = *pr.second;
        }
    }
}


    void countwords_config::read_conf(std::istream &cf) {
        using namespace std::literals::string_literals;

        file_to_options_map(cf);
        indir = get_option_from_map<decltype(indir)>("indir"s);
        out_by_a = get_option_from_map<decltype(out_by_a)>("out_by_a"s);
        out_by_n = get_option_from_map<decltype(out_by_n)>("out_by_n"s);

        max_file_size = get_numeric_option("max_file_size"s);
        indexing_threads  = get_numeric_option("indexing_threads"s);
        merging_threads = get_numeric_option("merging_threads"s);
        filenames_queue_size = get_numeric_option("filenames_queue_size"s);
        raw_files_queue_size = get_numeric_option("raw_files_queue_size"s);
        dictionaries_queue_size = get_numeric_option("dictionaries_queue_size"s);

        validate_conf();
    }

    void countwords_config::validate_conf() const {
        if (indir.empty()) {
            std::cerr << "Error: indir is empty" << std::endl;
            exit(5);
        } else if (indexing_extensions.empty()) {
            std::cerr << "Error: indexing_extensions is empty" << std::endl;
            exit(5);
        } else if (archives_extensions.empty()) {
            std::cerr << "Error: archives_extensions is empty" << std::endl;
            exit(5);
        } else if (out_by_a.empty()) {
            std::cerr << "Error: out_by_a is empty" << std::endl;
            exit(5);
        } else if (out_by_n.empty()) {
            std::cerr << "Error: out_by_n is empty" << std::endl;
            exit(5);
        }

    }

    countwords_config::countwords_config(std::istream & fs)
    {
        read_conf(fs);
        options_map = options_map_t{};
    }


template<typename T>
    T countwords_config::get_option_from_map(const std::string &option_name) const {
        if (options_map.count(option_name) == 0) {
            std::cerr << "Error: Option not found: " << option_name << std::endl;
            exit(5);
        }
        try {
            auto elem_itr = options_map.find(option_name);
            if (elem_itr == options_map.end()) {
                std::cerr << "Error: Option not found: " << option_name << std::endl;
                exit(5);
            }
            return from_str<T>(elem_itr->second);

        } catch (std::runtime_error &ex) {
            std::cerr << "Error: Wrong argument: " << option_name << std::endl;
            exit(5);
        }

    }

size_t countwords_config::get_numeric_option(const std::string &option_name) {
    int tmp = get_option_from_map<decltype(tmp)>(option_name);
    if (tmp < 0) {
        std::cerr << "Error: " << option_name << " is negative" << std::endl;
        exit(5);
    }
    return static_cast<size_t>(tmp);
}
