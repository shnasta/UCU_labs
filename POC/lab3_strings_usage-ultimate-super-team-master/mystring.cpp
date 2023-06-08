// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "mystring.h"


my_str_t::my_str_t(size_t size, char initial): size_m{size} {
    capacity_m = calc_capacity(size_m);
    data_m = new char[capacity_m];
    for (size_t i {}; i < size; ++i) {
        *(data_m + i) = initial;
    }
    data_m[size_m] = '\0';
}

my_str_t::my_str_t(const char *cstr): size_m{0} {
    while (*(cstr + size_m) != '\0') {
        size_m++;
    }

    capacity_m = calc_capacity(size_m);
    data_m = new char[capacity_m];

    for (size_t i {}; i < size_m; ++i) {
        *(data_m + i) = *(cstr + i);
    }
    data_m[size_m] = '\0';
}

my_str_t::my_str_t(const std::string &str) {
    size_m = str.size();
    capacity_m = calc_capacity(size_m);
    data_m = new char[capacity_m];

    for (size_t i {}; i < size_m; ++i) {
        *(data_m + i) = str[i];
    }
    data_m[size_m] = '\0';
}

my_str_t::my_str_t(const my_str_t &mystr) {
    size_m = mystr.size();
    capacity_m = mystr.capacity();
    data_m = new char[capacity_m];

    for (size_t i {}; i < size_m; ++i){
        *(data_m + i) = *(mystr.c_str() + i);
    }
    data_m[size_m] = '\0';
}

size_t my_str_t::calc_capacity(const size_t &size) {
    return ((size+1) / 16) * 16 + 15;
}

my_str_t &my_str_t::operator=(const my_str_t &mystr) {
    if (this == &mystr) return *this;
    delete[] data_m;

    size_m = mystr.size();
    capacity_m = mystr.capacity();

    data_m = new char[mystr.capacity()];

    for (size_t i {}; i < mystr.size(); ++i) {
        *(data_m + i) = *(mystr.c_str() + i);
    }
    data_m[mystr.size()] = '\0';

    return *this;
}

size_t my_str_t::capacity() const noexcept {
    return capacity_m;
}

size_t my_str_t::size() const noexcept {
    return size_m;
}

const char *my_str_t::c_str() const {
    return data_m;
}

char &my_str_t::operator[](size_t idx) {
    return *(data_m + idx);
}

const char &my_str_t::operator[](size_t idx) const {
    return *(data_m + idx);
}

char &my_str_t::at(size_t idx) {
    if (idx > size_m)
        throw std::out_of_range("Index is out of range!");
    return *(data_m + idx);
}

const char &my_str_t::at(size_t idx) const {
    if (idx > size_m) throw std::out_of_range("Index out of range!");
    return *(data_m + idx);
}

void my_str_t::reserve(size_t new_capacity) {
    if (new_capacity <= capacity_m) return;
    capacity_m = new_capacity;
    char *ptr = new char[capacity_m];
    memcpy(ptr, data_m, size_m);
    delete []data_m;
    data_m = ptr;
    data_m[size_m] = '\0';
}

void my_str_t::resize(size_t new_size, char new_char) {
    if (new_size<=size_m){
        size_m = new_size;
        data_m[size_m] = '\0';
        return;
    }
    else if (new_size > capacity_m){
        reserve(calc_capacity(new_size));
    }
    for (size_t i{size_m}; i < new_size; ++i) {
        *(data_m+i) = new_char;
    }
    size_m = new_size;
    data_m[size_m] = '\0';
}

void my_str_t::insert(size_t idx, const my_str_t &str) {
    if (idx >= size_m)
        throw std::out_of_range("Index is out of range!");
    if (str.size_m + size_m > capacity_m)
        reserve(calc_capacity(str.size_m + size_m));
    for (size_t i{1}; i<size_m-idx+1; ++i){ // move chars after idx to the right
        *(data_m + size_m + str.size() - i) = *(data_m + size_m - i);
    }
    for (size_t i{}; i<str.size(); ++i){ // insert chars form str
        *(data_m+idx+i) = *(str.c_str()+i);
    }
    size_m += str.size();
    data_m[size_m] = '\0';
}

void my_str_t::insert(size_t idx, const char *cstr) {
    if (idx >= size_m)
        throw std::out_of_range("Index is out of range!");
    size_t cstr_size = 0;
    while (*(cstr+cstr_size) != '\0') //calculating size of cstr
        cstr_size++;
    if (size_m + cstr_size > capacity_m)
        reserve(calc_capacity(size_m + cstr_size));
    for (size_t i{1}; i<size_m-idx+1; ++i){ // move chars after idx to the right
        *(data_m + size_m + cstr_size - i) = *(data_m + size_m - i);
    }
    for (size_t i{}; i<cstr_size; ++i){ // insert chars form str
        *(data_m+idx+i) = *(cstr+i);
    }
    size_m += cstr_size;
    data_m[size_m] = '\0';
}

void my_str_t::insert(size_t idx, char c) {
    if (idx > size_m) throw std::out_of_range("Index out of range!");

    if (size_m == capacity_m - 1) reserve(calc_capacity(size_m + 1));

    for (size_t i{1}; i < size_m - idx + 1; ++i) {
        *(data_m + size_m + 1 - i) = *(data_m + size_m - i);
    }

    *(data_m + idx) = c;
    size_m++;
    data_m[size_m] = '\0';
}

void my_str_t::append(char c) {
    if (size_m + 1 >= capacity_m)
        reserve(calc_capacity(size_m + 1));
    *(data_m + size_m) = c;
    size_m++;
    data_m[size_m] = '\0';
}

void my_str_t::append(const my_str_t &str) {
    size_t new_size = size_m + str.size();
    if (new_size >= capacity_m) reserve(calc_capacity(new_size));

    for (size_t i {size_m}; i < new_size; ++i) {
        *(data_m + i) = *(str.c_str() + (i - size_m));
    }

    size_m = new_size;
    data_m[size_m] = '\0';
}

void my_str_t::append(const char *cstr) {
    size_t new_size = 0;

    while (*(cstr + new_size) != '\0') {
        new_size++;
    }

    new_size += size_m;

    if (new_size >= capacity_m) reserve(calc_capacity(new_size));

    for (size_t i {size_m}; i < new_size; ++i) {
        *(data_m + i) = *(cstr + (i - size_m));
    }

    size_m = new_size;
    data_m[size_m] = '\0';
}

void my_str_t::clear() {
    if (size_m > 0) *data_m = '\0';
    size_m = 0;
}

void my_str_t::swap(my_str_t &other) noexcept {
    my_str_t temp = *this;
    *this = other;
    other = temp;
}

my_str_t::~my_str_t() {
    delete[] data_m;
}

size_t my_str_t::find(char c, size_t idx) {
    if (idx > size_m) throw std::out_of_range("Index out of range!");

    for (size_t i {idx}; i < size_m; ++i) {
        if (*(data_m + i) == c) {
            return i;
        }
    }

    return not_found;
}

size_t my_str_t::find(const char *cstr, size_t idx) {
    if (idx > size_m) throw std::out_of_range("Index out of range!");

    size_t cstr_size = 0;

    while (*(cstr + cstr_size) != '\0') {
        cstr_size++;
    }

    size_t i;
    int count = 0;

    for (i = idx; i < size_m; ++i) {
        if (count == cstr_size) {
            break;
        }
        if (*(cstr + count) == *(data_m + i)) {
            count++;
        } else {
            i -= count;
            count = 0;
        }

        if (count == cstr_size)
            return i + 1 - count;
    }

    return not_found;
}

void my_str_t::shrink_to_fit() {
    capacity_m = calc_capacity(size_m);
    char * temp_buffer = new char[capacity_m];

    for (size_t i {}; i < size_m; ++i) {
        *(temp_buffer + i) = data_m[i];
    }

    delete[] data_m;

    data_m = temp_buffer;
    data_m[size_m] = '\0';
}

void my_str_t::erase(size_t begin, size_t size) {
    if (begin >= size_m) throw std::out_of_range("Index is out of range!");
    if (size > size_m-begin){
        *(data_m + begin) = '\0';
        size_m = begin + 1;
    }
    else{
        for (size_t i{}; i <size_m - begin - size ; ++i) {
            *(data_m + begin + i) = *(data_m + begin + size + i);
        }
        size_m -= size;
        *(data_m + size_m) = '\0';
    }
    data_m[size_m] = '\0';
}

size_t my_str_t::find(const std::string &str, size_t idx) {
    if (idx > size_m) throw std::out_of_range("Index is out of range!");
    size_t count = 0;
    size_t i;
    for (i = idx; i < size_m; ++i) {
        if (str[count] == *(data_m+i))
            count++;
        else{
            i -= count;
            count = 0;
        }
        if (count == str.size())
            return i + 1 - count;
    }

    return not_found;
}

my_str_t my_str_t::substr(size_t begin, size_t size) {
    if (begin > size_m || begin + size > size_m)
        throw std::out_of_range("Index is out of range");
    char * res {new char[size+1]};
    for (size_t i{0}; i < size; ++i) {
        *(res + i) = *(data_m + begin + i);
    }
    res[size] = '\0';
    my_str_t tmp{res};
    delete[] res;
    return tmp;
}

my_str_t::my_str_t(my_str_t &&source) noexcept
    :data_m(source.data_m), size_m(source.size_m), capacity_m(source.capacity_m){
    source.size_m = 0;
    source.capacity_m = 0;
    source.data_m = nullptr;
}

my_str_t &my_str_t::operator=(my_str_t &&other)  noexcept {
    if (this != &other){
        delete[] data_m;
        data_m = other.data_m;
        size_m = other.size_m;

        other.data_m = nullptr;
        other.size_m = 0;
        other.capacity_m = 0;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& stream , const my_str_t &str){
    stream << str.c_str();
    return stream;
}

std::istream& operator>>(std::istream& stream, my_str_t &str){
    char *input = new char[4096];
    stream >> input;
    str = my_str_t{input};
    delete[] input;
    return stream;
}

bool operator==(const my_str_t &str1, const char *cstr2) {
    for (size_t i{}; i < str1.size()+1; ++i) { // size + 1 to check if it's the end of both strings
        if (str1[i] != *(cstr2 + i)) {
            return false;
        }
    }
    return true;
}

bool operator!=(const my_str_t &str1, const char *cstr2) {
    return !(str1 == cstr2);
}

bool operator>(const my_str_t &str1, const char *cstr2) {
    return !(str1<=cstr2);
}

bool operator<(const my_str_t &str1, const char *cstr2) {
    return !(str1>=cstr2);
}

bool operator>=(const my_str_t &str1, const char *cstr2) {
    for (size_t i{}; i < str1.size()+1; ++i) {
        if (str1[i] < *(cstr2 + i)) {
            return false;
        }
    }
    return true;
}

bool operator<=(const my_str_t &str1, const char *cstr2) {
    for (size_t i{}; i < str1.size()+1; ++i) {
        if (str1[i] > *(cstr2 + i)) {
            return false;
        }
    }
    return true;
}

bool operator==(const my_str_t& str1, const my_str_t& str2) {
    for (size_t i{}; i < str1.size() + 1; ++i) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }

    return true;
}

bool operator!=(const my_str_t& str1, const my_str_t& str2) {
    return !(str1 == str2);
}

bool operator>(const my_str_t& str1, const my_str_t& str2) {
    return !(str1 <= str2);
}

bool operator<(const my_str_t& str1, const my_str_t& str2) {
    return !(str1 >= str2);
}

bool operator>=(const my_str_t& str1, const my_str_t& str2) {
    for (size_t i{}; i < str1.size()+1; ++i) {
        if (str1[i] < str2[i]) {
            return false;
        }
    }
    return true;
}

bool operator<=(const my_str_t& str1, const my_str_t& str2) {
    for (size_t i{}; i < str1.size()+1; ++i) {
        if (str1[i] > str2[i]) {
            return false;
        }
    }
    return true;
}

bool operator==(const char* cstr2, const my_str_t& str1) {
    return str1 == cstr2;
}

bool operator!=(const char* cstr2, const my_str_t& str1) {
    return str1 != cstr2;
}

bool operator>(const char* cstr2, const my_str_t& str1) {
    return str1 < cstr2;
}

bool operator>=(const char* cstr2, const my_str_t& str1) {
    return str1 <= cstr2;
}

bool operator<(const char* cstr2, const my_str_t& str1) {
    return str1 > cstr2;
}

bool operator<=(const char* cstr2, const my_str_t& str1) {
    return str1 >= cstr2;
}

std::istream& readline(std::istream& stream, my_str_t& str) {
    char * temp_arr = new char[4096]{};
    stream.get(temp_arr, 4096, '\n');
    my_str_t temp_str {temp_arr};
    delete[] temp_arr;
    str = temp_str;
    return stream;
}

my_str_t& operator+=(my_str_t& str1, my_str_t& str2) {
    size_t new_size = str1.size() + str2.size() + 1;
    char * temp_buffer = new char[new_size]{};

    for (size_t i {}; i < str1.size(); ++i) {
        *(temp_buffer + i) = str1[i];
    }

    for (size_t i {}; i < str2.size(); ++i) {
        *(temp_buffer + i + str1.size()) = str2[i];
    }

    my_str_t temp_str {temp_buffer};
    delete[] temp_buffer;
    str1 = temp_str;

    return str1;
}

my_str_t& operator+(my_str_t& str1, my_str_t& str2) {
    return str1 += str2;
}

my_str_t& operator*=(my_str_t& str1, size_t num) {
    if (num == 0) {
        str1.clear();
        return str1;
    }

    size_t new_size = str1.size() * num + 1;
    char * temp_buffer = new char[new_size]{};
    size_t counter {0};

    for (size_t i {}; i < num; ++i) {
        for (size_t j {0}; j < str1.size(); ++j) {
            *(temp_buffer + counter) = str1[j];
            counter++;
        }
    }

    my_str_t temp_str {temp_buffer};
    delete [] temp_buffer;
    str1 = temp_str;

    return str1;
}

my_str_t& operator*(my_str_t& str1, size_t num) {
    return str1 *= num;
}
