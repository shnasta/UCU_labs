// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "parse_config.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <filesystem>


auto parse_config::parse_string(std::string arg) {
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



void parse_config::file_to_options_map(std::istream& cf) {
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

        options_map[pr.first] = *pr.second;
    }
}


void parse_config::read_conf(std::istream &cf) {
    using namespace std::literals::string_literals;

    file_to_options_map(cf);

    dens = get_option_from_map<decltype(dens)>("dens"s);
    h_cap = get_option_from_map<decltype(h_cap)>("h_cap"s);
    conduct = get_option_from_map<decltype(conduct)>("conduct"s);
    width = get_option_from_map<decltype(width)>("width"s);
    height = get_option_from_map<decltype(height)>("height"s);

    dx = get_option_from_map<decltype(dx)>("dx"s);
    dy = get_option_from_map<decltype(dy)>("dy"s);
    dt = get_option_from_map<decltype(dt)>("dt"s);
    dt_gif = get_option_from_map<decltype(dt_gif)>("dt_gif"s);
    first_state_path = get_option_from_map<decltype(first_state_path)>("first_state_path"s);
    total_time = get_option_from_map<decltype(total_time)>("total_time"s);

    alpha = static_cast<double>(conduct) / (dens * h_cap);
    num_iters = static_cast<int>(total_time/ dt);
    iter_gif = static_cast<int>(dt_gif / dt);

    validate_conf();
}

void parse_config::validate_conf() const {
    if (first_state_path.empty()) {
        std::cerr << "Error: first_state_path is empty" << std::endl;
        exit(5);
    }
    if (!std::filesystem::exists(first_state_path)) {
        std::cerr << "Error: first_state_path does not exist" << std::endl;
        exit(5);
    }
    if (!std::filesystem::is_regular_file(first_state_path)) {
        std::cerr << "Error: first_state_path is not a file" << std::endl;
        exit(5);
    }

    if (dt > std::max(dx, dy)* std::max(dx, dy)/ (4 * alpha)) {
        std::cerr << "Error: von Neumann's condition is not satisfied" << std::endl;
        exit(5);
    }

    if (dt_gif < dt) {
        std::cerr << "Error: dt_gif < dt" << std::endl;
        exit(5);
    }

    check_numeric(dx, "dx");
    check_numeric(dy, "dy");
    check_numeric(dt, "dt");
    check_numeric(dt_gif, "dt_gif");
    check_numeric(dens, "dens");
    check_numeric(h_cap, "h_cap");
    check_numeric(conduct, "conduct");
    check_numeric(width, "width");
    check_numeric(height, "height");
}

template<typename T>
void parse_config::check_numeric(T num, const std::string& option_name) const {
    if (num <= 0) {
        std::cerr << "Error: " << option_name << " <= 0" << std::endl;
        exit(5);
    }
}

parse_config::parse_config(std::istream & fs)
{
    read_conf(fs);
    options_map = options_map_t{};
}


template<typename T>
T parse_config::get_option_from_map(const std::string &option_name) const {
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
