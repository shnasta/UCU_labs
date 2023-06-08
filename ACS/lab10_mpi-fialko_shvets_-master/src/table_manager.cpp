// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "table_manager.hpp"

#include <utility>

table_manager::table_manager(std::unique_ptr<parse_config>& input_config, std::vector<double> init_list) {
    content_list = std::move(init_list);
    config_ptr = std::move(input_config);
}

double table_manager::get_value(int x , int y){
    return  content_list[config_ptr->width * y + x];
}

void table_manager::set_value(int x, int y, double value) {
    content_list[config_ptr->width * y + x] = value;
}

std::vector<double> table_manager::get_content_list() {
    return content_list;
}

void table_manager::print_table(std::ofstream& output_file) {
    for (int i = 0; i < config_ptr->height; ++i) {
        for (int j = 0; j < config_ptr->width; ++j) {
            output_file << get_value(j, i) << " ";
        }
        output_file << std::endl;
    }
}

double table_manager::get_next_step_temperature(int x, int y) {
    auto current_temperature = get_value(x, y);
    auto left_temperature = get_value(x - 1, y);
    auto right_temperature = get_value(x + 1, y);
    auto top_temperature = get_value(x, y - 1);
    auto bottom_temperature = get_value(x, y + 1);
    auto dx_2 = config_ptr->dx * config_ptr->dx;
    auto dy_2 = config_ptr->dy * config_ptr->dy;

    double new_temperature = current_temperature + config_ptr->alpha * config_ptr->dt * (
            (left_temperature - 2 * current_temperature + right_temperature) / dx_2 +
            (top_temperature - 2 * current_temperature + bottom_temperature) / dy_2
            );

    return new_temperature;
}

void table_manager::make_step() {
    std::vector<double> next_content_list;
    next_content_list.resize(config_ptr->width * config_ptr->height);

    // set border values
    for (int i = 0; i < config_ptr->width; ++i) {
        next_content_list[i] = get_value(i, 0);
        next_content_list[config_ptr->width * (config_ptr->height - 1) + i] = get_value(i, config_ptr->height - 1);
    }
    for (int i = 0; i < config_ptr->height; ++i) {
        next_content_list[config_ptr->width * i] = get_value(0, i);
        next_content_list[config_ptr->width * i + config_ptr->width - 1] = get_value(config_ptr->width - 1, i);
    }

    // set inner values
    for (int i = 1; i < config_ptr->width - 1; ++i) {
        for (int j = 1; j < config_ptr->height - 1; ++j) {
            next_content_list[config_ptr->width * j + i] = get_next_step_temperature(i, j);
        }
    }

    content_list = std::move(next_content_list);
}

std::vector<double> table_manager::get_second_line() {
    std::vector<double> second_line;
    second_line.resize(config_ptr->width);
    for (int i = 0; i < config_ptr->width; ++i) {
        second_line[i] = get_value(i, 1);
    }
    return second_line;
}

void table_manager::set_first_line(std::vector<double> new_line) {
    for (int i = 0; i < config_ptr->width; ++i) {
        set_value(i, 0, new_line[i]);
    }
}

std::vector<double> table_manager::get_pre_last_line() {
    std::vector<double> pre_last_line;
    pre_last_line.resize(config_ptr->width);
    for (int i = 0; i < config_ptr->width; ++i) {
        pre_last_line[i] = get_value(i, config_ptr->height - 2);
    }
    return pre_last_line;
}

void table_manager::set_last_line(std::vector<double> new_line) {
    for (int i = 0; i < config_ptr->width; ++i) {
        set_value(i, config_ptr->height - 1, new_line[i]);
    }
}