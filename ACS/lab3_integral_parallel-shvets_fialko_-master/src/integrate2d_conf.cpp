// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "integrate2d_conf.hpp"

#include <algorithm>
#include <iostream>
#include <boost/algorithm/string.hpp>


auto integrate1d_conf::parse_string(std::string arg){
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
    return parse_string_ret_t{left_part, right_part};
}

void integrate1d_conf::file_to_options_map(std::istream& cf){
    std::string line;
    while( std::getline(cf, line) ){
        auto pr = parse_string(line);
        if(pr.first.empty()) {
            if(!pr.second)
                continue;
            else
                throw wrong_option_line_error{"Wrong config line -- no option name: " + line}; // "=..."
        } else if(!pr.second){
            throw wrong_option_line_error{"Wrong config line -- no '=': " + line}; // "abc" -- no '='
        } else if(pr.second->empty()){
            throw wrong_option_arg_error{"Wrong config option value: " + line}; // "key="
        }
        if( options_map.count(pr.first) ){
            throw option_duplicated_error{"Duplicated option name: " + pr.first + " = "
                                          + *pr.second + "; prev. val: " + options_map[pr.first] };
        }
        options_map[pr.first] = *pr.second;
    }
}



void integrate1d_conf::read_conf(std::istream& cf)
{
    using namespace std::literals::string_literals;

    file_to_options_map(cf);

    rel_err = get_option_from_map<decltype(rel_err)>("rel_err"s);
    abs_err = get_option_from_map<decltype(abs_err)>("abs_err"s);
    x1 = get_option_from_map<decltype(x1)>("x_start"s);
    x2 = get_option_from_map<decltype(x2)>("x_end"s);
    y1 = get_option_from_map<decltype(y1)>("y_start"s);
    y2 = get_option_from_map<decltype(y2)>("y_end"s);
    initial_steps_x = get_option_from_map<decltype(initial_steps_x)>("init_steps_x"s);
    initial_steps_y = get_option_from_map<decltype(initial_steps_y)>("init_steps_y"s);
    max_iters = get_option_from_map<decltype(max_iters)>("max_iter"s);
    max_steps_x = initial_steps_x * (static_cast<size_t>(2) << max_iters);
    max_steps_y = initial_steps_y * (static_cast<size_t>(2) << max_iters);

    validate_conf();
}

void integrate1d_conf::validate_conf() const{
    if( x2 <= x1 ) {
        throw wrong_logical_option_arg_error("x1 should be <= x2");
    }
    if (y2 <= y1) {
        throw wrong_logical_option_arg_error("y1 should be <= y2");
    }
    if( initial_steps_x < 10 ) {
        throw wrong_logical_option_arg_error("Too few initial_steps_x");
    }
    if (initial_steps_x > max_steps_x) {
        throw wrong_logical_option_arg_error("Too many initial_steps_x");
    }
    if (initial_steps_y < 10) {
        throw wrong_logical_option_arg_error("Too few initial_steps_y");
    }
    if (initial_steps_y > max_steps_y) {
        throw wrong_logical_option_arg_error("Too many initial_steps_y");
    }
    if( max_iters < 3 ) {
        throw wrong_logical_option_arg_error("Too small max_iters");
    }
    if( max_iters > 30 ){
        throw wrong_logical_option_arg_error("Too large max_iters");
    }
    if( abs_err <=0 || rel_err <= 0 )
    {
        throw wrong_logical_option_arg_error("Errors should be positive");
    }
}

integrate1d_conf::integrate1d_conf(std::istream& fs){
    read_conf(fs);
    options_map = options_map_t{};
}

template<typename T>
T integrate1d_conf::get_option_from_map(const std::string& option_name) const{
    if( options_map.count(option_name) == 0 ){
        throw option_not_found_error("Option not found: " + option_name);
    }
    try {
        auto elem_itr = options_map.find(option_name);
        if( elem_itr != options_map.end() ) {
            return from_str<T>(elem_itr->second);
        } else {
            throw wrong_option_arg_error{"Option " + option_name + " not found"};
        }
    }catch( std::runtime_error& ex ){
        throw wrong_option_arg_error{ex.what()};
    }
}
