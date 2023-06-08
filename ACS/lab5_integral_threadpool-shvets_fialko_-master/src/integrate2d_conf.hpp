#ifndef TEST_INTEGRATE2D_CONF_HPP
#define TEST_INTEGRATE2D_CONF_HPP

#include <string>
#include <optional>
#include <map>
#include <iosfwd>
#include <exception>
#include <stdexcept>

#ifdef PRINT_INTERMEDIATE_STEPS
#include <iostream>
#endif

#include "string_tools.hpp"

struct config_error : public std::runtime_error {
    using runtime_error::runtime_error;
};

struct wrong_option_line_error : public config_error{
    using config_error::config_error;
};

struct wrong_option_arg_error : public config_error{
    using config_error::config_error;
};

struct option_duplicated_error : public config_error{
    using config_error::config_error;
};

struct option_not_found_error : public config_error{
    using config_error::config_error;
};

struct wrong_logical_option_arg_error : public wrong_option_arg_error{
    using wrong_option_arg_error::wrong_option_arg_error;
};



class integrate2d_conf {
public:
    double rel_err = 0.0, abs_err = 0.0;
    double x1 = 0.0, x2 = 0.0;
    double y1 = 0.0, y2 = 0.0;
    size_t initial_steps_x = 0, initial_steps_y = 0;
    size_t max_steps_x = 0, max_steps_y = 0;
    size_t max_iters = 0;

    explicit integrate2d_conf(std::istream& fs);
private:
    using parse_string_ret_t = std::pair<std::string, std::optional<std::string>>;
    using options_map_t = std::map<std::string, std::string>;

    static auto parse_string(std::string arg);
    void file_to_options_map(std::istream& cf);

    template<typename T>
    T get_option_from_map(const std::string& option_name) const;

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
