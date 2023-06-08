
#ifndef TEST_STRING_TOOLS_HPP
#define TEST_STRING_TOOLS_HPP

#include <string>
#include <sstream>

template<typename T>
auto from_str(const std::string& arg){
    T res = T{};
    std::stringstream ss{arg};
    ss >> res;
    if( std::string temp; ss >> temp ){
        throw std::runtime_error{"Wrong numerical value: " + arg};
    }
    return res;
}


template<>
inline auto from_str<unsigned long long>(const std::string& arg){
    size_t last_sym = 0;
    unsigned long long res = std::stoull(arg, &last_sym);

    if( last_sym != arg.size() ){
        throw std::runtime_error{"Wrong numerical value: " + arg};
    }
    return res;
}

#endif //TEST_STRING_TOOLS_HPP
