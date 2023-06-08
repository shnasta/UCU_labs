// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#ifndef TEMPLATE_PARSE_TABLE_H
#define TEMPLATE_PARSE_TABLE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class parse_table {

private:
    int width;
    int height;
    std::vector<double> table;

public:
    parse_table(int w, int h);
    void read_table(const std::string& filename);
    void print_table();
    std::vector<double> get_table();
    std::vector<double> get_part(int part_idx, int num_parts);
    int get_part_height(int part_idx, int num_parts);
};


#endif //TEMPLATE_PARSE_TABLE_H
