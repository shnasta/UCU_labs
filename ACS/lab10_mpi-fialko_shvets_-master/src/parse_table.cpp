// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#include "parse_table.hpp"

parse_table::parse_table(int w, int h) : width(w), height(h) {
    table.resize(w * h);
}

void parse_table::read_table(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return;
    }

    int value;
    for (int i = 0; i < width * height && file >> value; ++i) {
        table[i] = value;
    }

    file.close();
}

void parse_table::print_table() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cout << table[i * width + j] << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<double> parse_table::get_table() {
    return table;
}

int parse_table::get_part_height(int part_idx, int num_parts) {
    auto pure_height = height / num_parts + (part_idx == num_parts - 1 ? height % num_parts : 0);
    return pure_height + (part_idx == 0 || part_idx == num_parts - 1 ? 1 : 2);
}

std::vector<double> parse_table::get_part(int part_idx, int num_parts) {
    int part_height = get_part_height(part_idx, num_parts);

    int start_row = part_idx * (height / num_parts);
    if (part_idx != 0) {
        start_row -= 1;
    }

    std::vector<double> part(part_height * width);
    for (int i = 0; i < part_height; ++i) {
        for (int j = 0; j < width; ++j) {
            part[i * width + j] = table[(start_row + i) * width + j];
        }
    }

    return part;
}