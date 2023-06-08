#include <iostream>
#include <fstream>
#include <vector>
#include "mystring.h"

void edit_text(std::vector<my_str_t> &words, size_t max_value){
    my_str_t additions("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    size_t current = 0;
    for (my_str_t &word: words) {
        size_t difference = max_value - word.size();
        for (size_t i = 0; i< difference; ++i) {
            word.append(additions.at((i + current)%52));
        }
        current += difference;
        current = current % 52;
    }
}

int main(int argc, char *argv[])
{
    std::string file_name = argv[1];
    std::vector<my_str_t> vector_of_words;
    std::ifstream file(file_name);

    my_str_t word("");
    size_t max_length = 0;
    while (file >> word)
    {
        if (word.size() >= max_length) {
            max_length = word.size();
        }
        vector_of_words.push_back(word);
    }
    file.close();


    edit_text(vector_of_words, max_length);

    std::ofstream file_out(argv[2]);
    for (my_str_t word : vector_of_words) {
        file_out << word << std::endl;
    }
    file_out.close();
}
