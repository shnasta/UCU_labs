#ifndef TEST_INTEGRATE2D_CONF_HPP
#define TEST_INTEGRATE2D_CONF_HPP

#include <string>
#include <optional>
#include <map>
#include <iosfwd>
#include <exception>
#include <stdexcept>
#include <vector>

#ifdef PRINT_INTERMEDIATE_STEPS
#include <iostream>
#endif

#include "string_tools.hpp"

class countwords_config {
public:
    std::string indir, out_by_a, out_by_n;
    std::vector<std::string> indexing_extensions, archives_extensions;
    size_t max_file_size = 0;
//    size_t indexing_threads, merging_threads;
//    size_t filenames_queue_size, raw_files_queue_size, dictionaries_queue_size;

    explicit countwords_config(std::istream& fs);
private:
    using parse_string_ret_t = std::pair<std::string, std::optional<std::string>>;
    using options_map_t = std::map<std::string, std::string>;

    static auto parse_string(std::string arg);
    void file_to_options_map(std::istream& cf);

    template<typename T>
    T get_option_from_map(const std::string& option_name) const;
    size_t get_numeric_option(const std::string &option_name);
    void read_conf(std::istream& cf);
    void validate_conf() const;
    options_map_t options_map;
};


#ifdef PRINT_INTERMEDIATE_STEPS
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& arg)
{
    if(arg) os << *arg;
    return os;
}
#ifdef __GNUC__ // Clang too
template<typename T>
void print_type() { std::cout << __PRETTY_FUNCTION__ << '\n'; }
#endif
#endif


#endif //TEST_INTEGRATE2D_CONF_HPP
