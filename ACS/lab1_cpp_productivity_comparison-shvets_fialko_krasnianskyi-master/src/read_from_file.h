#ifndef TEMPLATE_READ_FROM_FILE_H
#define TEMPLATE_READ_FROM_FILE_H

#include <iostream>
#include <string>
#include <vector>


std::vector<std::string>  read_from_file(const std::string& method, const std::string& file_path);

std::vector<std::string> first_method(const std::string& file_path);
std::vector<std::string> second_method(const std::string& file_path);
std::vector<std::string> third_method(const std::string& file_path);
std::vector<std::string> fourth_method(const std::string& file_path);
std::vector<std::string> fifth_method(const std::string& file_path);

void split(const std::string& str, std::vector<std::string>& result);
std::ifstream open_file(const std::string& file_path);


#endif //TEMPLATE_READ_FROM_FILE_H
