// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "read_from_file.h"
#include <fstream>
#include <sstream>
#include <limits>

std::vector<std::string> read_from_file(const std::string& method, const std::string& file_path) {
    if (method == "1") {
        return first_method(file_path);
    } else if (method == "2") {
        return second_method(file_path);
    } else if (method == "3") {
        return third_method(file_path);
    } else if (method == "4") {
        return fourth_method(file_path);
    } else if (method == "5") {
        return fifth_method(file_path);
    } else {
        std::cout << "Invalid method number" << '\n';
        exit(2);
    }
}

std::vector<std::string> first_method(const std::string& file_path) {
    std::vector<std::string> result;

    std::ifstream infile = open_file(file_path);
    std::string line;

    try {
        while (infile >> line) {
            result.push_back(line);
        }
    }
    catch (std::exception& e) {
        std::cout << "Error while reading file" << '\n';
        exit(5);
    }

    return result;
}

std::vector<std::string> second_method(const std::string& file_path) {
    std::ifstream infile = open_file(file_path);

    auto file_stream = std::stringstream{};
    file_stream << infile.rdbuf();

    std::vector<std::string> result;
    std::string substr;

    try {
        while (file_stream >> substr) {
            result.push_back(substr);
        }
    }
    catch (std::exception& e) {
        std::cout << "Error while reading file" << '\n';
        exit(5);
    }

    return result;
}

std::vector<std::string> third_method(const std::string& file_path) {
    std::ifstream infile = open_file(file_path);

    auto file_stream = std::stringstream{};
    file_stream << infile.rdbuf();

    std::vector<std::string> result;

    try {
        while (file_stream >> result.emplace_back()){};
    }
    catch (std::exception& e) {
        std::cout << "Error while reading file" << '\n';
        exit(5);
    }

    return result;
}

std::vector<std::string> fourth_method(const std::string& file_path) {
    std::vector<std::string> result;
    std::ifstream infile = open_file(file_path);

    try {
        auto const start_pos = infile.tellg();
        infile.ignore(std::numeric_limits<std::streamsize>::max());
        auto const char_count = infile.gcount();
        infile.seekg(start_pos);
        auto s = std::string(char_count, char{});
        infile.read(&s[0], s.size());
        split(s, result);
    }
    catch (std::exception& e) {
        std::cout << "Error while reading file" << '\n';
        exit(5);
    }

    return result;
}

std::vector<std::string> fifth_method(const std::string& file_path) {
    std::vector <std::string> result;
    std::ifstream infile = open_file(file_path);

    try {
        auto s = std::string{};
        s.assign(std::istreambuf_iterator < char > {infile},
                 std::istreambuf_iterator < char > {});
        split(s, result);
    }
    catch (std::exception& e) {
        std::cout << "Error while reading file" << '\n';
        exit(5);
    }

    return result;
}

std::ifstream open_file(const std::string& file_path) {
    std::ifstream infile(file_path);
    if (! infile.is_open()) {
        std::cout << "Error while opening input file" << "\n";
        exit(3);
    }
    return infile;
}

void split(const std::string& str, std::vector<std::string>& result) {
    size_t first = 0;

    while (first < str.size()) {
        size_t second = first;
        while (second < str.size() && str[second] != '\n')  {
            ++second;
        }

        result.emplace_back(str.begin() + static_cast<int>(first), str.begin() + static_cast<int>(second));

        if (second == str.size()) {
            break;
        }
        first = second + 1;
    }
}
