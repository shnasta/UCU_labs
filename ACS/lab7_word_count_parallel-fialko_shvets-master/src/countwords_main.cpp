#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include "countwords_config.hpp"
#include "countwords.hpp"

int main(int argc, char* argv[]) {
    std::string filename;
    if (argc < 2) {
        std::cerr << "Error: config path is not specified." << std::endl;
        return 1;
    } else {
        filename = argv[1];
    }

    std::ifstream config_stream(filename);
    if (!config_stream.is_open()) {
        std::cerr << "Failed to open configuration file " << filename << std::endl;
        return 3;
    }

    std::unique_ptr<countwords_config> config_ptr;
    try {
        config_ptr = std::make_unique<countwords_config>(config_stream);
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 5;
    }


    CountWords count_words(std::move(config_ptr));
    count_words.manager();
    count_words.write();

    count_words.print_time();
}