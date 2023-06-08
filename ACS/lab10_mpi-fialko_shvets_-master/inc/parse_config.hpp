// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#ifndef PARSE_CONFIG_HPP
#define PARSE_CONFIG_HPP

#include <string>
#include <optional>
#include <map>
#include <iosfwd>
#include <exception>
#include <stdexcept>
#include <vector>


#include "string_tools.hpp"

class parse_config {
public:
    int dens, h_cap, conduct, num_iters, iter_gif;
    int height, width;
    double dx, dy, dt, dt_gif, total_time, alpha;
    std::string first_state_path;

    explicit parse_config(std::istream& fs);
private:
    using parse_string_ret_t = std::pair<std::string, std::optional<std::string>>;
    using options_map_t = std::map<std::string, std::string>;

    static auto parse_string(std::string arg);
    void file_to_options_map(std::istream& cf);

    template<typename T>
    T get_option_from_map(const std::string& option_name) const;
    void read_conf(std::istream& cf);
    void validate_conf() const;
    template<typename T>
    void check_numeric(T num, const std::string& option_name) const;
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


#endif //PARSE_CONFIG_HPP
