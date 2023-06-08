// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "parse_from_string.h"
#include <sstream>
#include <cstdlib>
#include <charconv>

std::vector<unsigned> parse_from_string(const std::string& method, const std::vector<std::string>& stings) {
    if (method == "1") {
        return first_method(stings);
    } else if (method == "2") {
        return second_method(stings);
    } else if (method == "3") {
        return third_method(stings);
    } else if (method == "4") {
        return fourth_method(stings);
    } else if (method == "5") {
        return fifth_method(stings);
    } else {
        std::cout << "Invalid method number" << '\n';
        exit(2);
    }
}

std::vector<unsigned> first_method(const std::vector<std::string>& stings) {
    std::vector<unsigned> result;

    for (auto& str : stings) {
        auto ss = std::stringstream(str);
        ss >> result.emplace_back();
    }

    return result;
}

std::vector<unsigned> second_method(const std::vector<std::string>& strings) {
    std::vector<unsigned> result;

    for (const std::string& s : strings) {
        result.emplace_back(std::atoi(s.c_str()));
    }

    return result;
}

std::vector<unsigned> third_method(const std::vector<std::string>& strings) {
    std::vector<unsigned> result;

    result.reserve(strings.size());
    for (const std::string& str: strings) {
        result.emplace_back(std::stod(str));
    }

    return result;
}

std::vector<unsigned> fourth_method(const std::vector<std::string>& strings) {
    std::vector<unsigned> result;

    for (const std::string& str: strings) {
        std::from_chars(str.data(), str.data() + str.size(), result.emplace_back());
    }

    return result;
}


std::vector<unsigned> fifth_method(const std::vector<std::string>& strings) {
    std::vector<unsigned> result;

    for (const std::string& str: strings) {
        sscanf(str.c_str(), "%u", &result.emplace_back());
    }

    return result;
}


