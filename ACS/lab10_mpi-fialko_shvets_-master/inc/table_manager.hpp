// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef MPI_TABLE_MANAGER_HPP
#define MPI_TABLE_MANAGER_HPP

#include <vector>
#include <memory>
#include <fstream>
#include "parse_config.hpp"

class table_manager {
private:
    std::vector<double> content_list;
    std::unique_ptr<parse_config> config_ptr;
public:
    table_manager(std::unique_ptr<parse_config>& input_config, std::vector<double> init_list);
    double get_value(int x , int y);
    void set_value(int x, int y, double value);
    std::vector<double> get_content_list();
    std::vector<double> get_second_line();
    std::vector<double> get_pre_last_line();
    void set_first_line(std::vector<double> new_line);
    void set_last_line(std::vector<double> new_line);

    void print_table(std::ofstream& output_file);

    double get_next_step_temperature(int x, int y);
    void make_step();
};


#endif //MPI_TABLE_MANAGER_HPP
