#include "mystring.h"
#include <cmath>


//Made by: Kazymyr Arsenii
//Calculate how much memmory should be allocated
size_t my_str_t::calculate_capacity(size_t size) {
//    if (2*size < 15) {
//        return 15;
//    }
//    return 2*size;
    return (size > 7) ? static_cast<size_t>(std::pow(2, std::ceil(std::log2(size + 1))) - 1) : 15;}

size_t my_str_t::calc_min_capacity(size_t size) {
    return (1 + size/16) * 16 - 1;
}

//Made by: Kazymyr Arsenii
my_str_t::my_str_t(size_t size, char initial) {
    size_m = size;
    capacity_m = calculate_capacity(size_m);
    data_m = new  char[capacity_m + 1];
    for (size_t i = 0; i < size_m; i++) {
        data_m[i] = initial;
    }

    data_m[size_m] = '\0';
}

my_str_t::my_str_t(const char* cstr) {
    size_m = strlen(cstr);
    capacity_m = calculate_capacity(size_m);
    data_m = new char[capacity_m + 1];
    for (size_t i = 0; i < size_m; i++) {
        data_m[i] = cstr[i];
    }
    data_m[size_m] = '\0';
}

my_str_t::my_str_t(const std::string& str) {
    size_m = str.length();
    capacity_m = calculate_capacity(size_m);
    data_m = new char[capacity_m + 1];
    for (size_t i = 0; i < size_m; i++) {
        data_m[i] = str[i];
    }
    data_m[size_m] = '\0';
}

my_str_t::my_str_t(const my_str_t& mystr) {
    size_m = mystr.size_m;
    capacity_m = mystr.capacity_m;
    data_m = new char[capacity_m + 1];
    for (size_t i = 0; i < size_m; i++) {
        data_m[i] = mystr[i];
    }
    data_m[size_m] = '\0';
}

size_t my_str_t::strlen(const char *cstr) {
    int count = 0;
    while(*cstr!='\0')
    {
        count++;
        cstr++;
    }
    return count;
}
//next
// 
//Made by: Sofia Knyshoyid
// move constructor
my_str_t::my_str_t(my_str_t&& mystr) noexcept{
    data_m = mystr.data_m;
    size_m = mystr.size_m;
    capacity_m = mystr.capacity_m;

    mystr.data_m = nullptr;
    mystr.size_m = 0;
    mystr.capacity_m = 0;
}

//Made by: Sofia Knyshoyid
// move assignment operator
my_str_t& my_str_t::operator=(my_str_t&& mystr) noexcept{
    if (this!= &mystr){
        delete[] data_m;

        data_m = mystr.data_m;
        size_m = mystr.size_m;
        capacity_m = mystr.capacity_m;

        mystr.data_m = nullptr;
        mystr.size_m = 0;
        mystr.capacity_m = 0;
    }

    return *this;
}
//Nastia Shvets
my_str_t& my_str_t::operator=(const my_str_t& mystr) {
    my_str_t sample(mystr);
    swap(sample);
    return *this;
}

//Nastia Shvets
void my_str_t::swap(my_str_t& other) noexcept {
    size_t sample = size_m;
    size_m = other.size_m;
    other.size_m = sample;

    size_t sample1 = capacity_m;
    capacity_m = other.capacity_m;
    other.capacity_m = sample1;

    char *sample2 = data_m;
    data_m = other.data_m;
    other.data_m = sample2;
}
// Kazymyr Arsenii
char& my_str_t::operator [ ] (size_t idx) {
    return data_m[idx%size()];
}

const char& my_str_t::operator [ ] (size_t idx) const {
    const char& output = data_m[idx % size()];
    return output;
}

char& my_str_t::at(size_t idx) {
    if ((idx >= size_m) || (idx < 0)) {
        throw std::out_of_range("Index is out of range");
    }
    return data_m[idx];
}

const char& my_str_t::at(size_t idx) const {
    if ((idx >= size_m)||(idx < 0)) {
        throw std::out_of_range("Index is out of range");
    }
    const char& output = data_m[idx];
    return output;
}

//next student
//Made by: Sofia Knyshoyid
void my_str_t::reserve(size_t new_capacity) {
    if (new_capacity>=capacity_m){
        char* data_new = new char[new_capacity+1];
        for (int i=0; i<capacity_m; ++i) {
            data_new[i] = data_m[i];
        }
        delete[] data_m;
        data_m = data_new;
        capacity_m = new_capacity;
        data_m[size_m] = '\0';
    }
}

//Made by: Sofia Knyshoyid
void my_str_t::shrink_to_fit(){
    size_t capacity_new = calculate_capacity(size_m);
    if (capacity_new!=capacity_m){
        char* data_new = new char[capacity_new+1];
        for (int i=0; i<capacity_new; ++i){
            data_new[i] = data_m[i];
        }
        delete[] data_m;
        data_m = data_new;
        capacity_m = capacity_new;
        data_m[size_m] = '\0';
    }
}


//Made by: Sofia Knyshoyid
void my_str_t::resize(size_t new_size , char new_char){
    if (new_size<size_m){
        size_m = new_size;
    }
    else{
        if (new_size>capacity_m) {
            reserve(calculate_capacity(new_size));
        }
        for (size_t n = size_m; n<new_size; ++n){
            data_m[n] = new_char;
        }
        size_m = new_size;

    }
    data_m[new_size] = '\0';
}

//Made by: Sofia Knyshoyid
void my_str_t::clear() {
    size_m = 0;
    delete[] data_m;
    capacity_m = calc_min_capacity(size_m);
    data_m = new char[capacity_m + 1];
    data_m[0] = '\0';
}

//Made by: Sofia Knyshoyid
void my_str_t::insert(size_t idx, const my_str_t& str) {
    if (idx > size_m) {
        throw std::out_of_range("Index is out of range");
    }
    size_t add_size = str.size();
    size_m = size_m + add_size;
    if (size_m >= capacity_m) {
        capacity_m = calculate_capacity(size_m);
        char *new_data = new char[capacity_m + 1];

        for (int i = 0; i < size_m; i++) {
            new_data[i] = data_m[i];
        }
        delete[] data_m;
        data_m = new_data;
    }
    for (size_t j = size_m; j > idx + add_size; --j){
        data_m[j -1] = data_m[j - 1 - add_size];
    }
    for (int g = idx;g < idx + add_size;++g){
        data_m[g] = str[g - idx];
    }
    data_m[size_m] = '\0';
}

//Made by: Sofia Knyshoyid
void my_str_t::insert(size_t idx , char c) {
    if (idx > size_m) {
        throw std::out_of_range("Index is out of range");
    }
    size_m += 1;
    if (size_m > capacity_m) {
        capacity_m = calculate_capacity(size_m);
        char *new_data = new char[capacity_m + 1];
        for (int i = 0; i < size_m - 1; i++) {
            new_data[i] = data_m[i];
        }
        delete[] data_m;
        data_m = new_data;
    }
    for (int i=size_m-1; i>idx; --i){
        data_m[i] = data_m[i-1];
    }
    data_m[idx] = c;
    data_m[size_m] = '\0';
}

//Made by: Sofia Knyshoyid
void my_str_t::insert(size_t idx , const char* cstr){
    if (idx > size_m) {
        throw std::out_of_range("Index is out of range");
    }
    size_t add_size = strlen(cstr);

    size_m = size_m + add_size;
    if (size_m >= capacity_m) {
        capacity_m = calculate_capacity(size_m);
        char *new_data = new char[capacity_m + 1];

        for (int i = 0; i < size_m; i++) {
            new_data[i] = data_m[i];
        }
        delete[] data_m;
        data_m = new_data;
    }
    for (size_t j = size_m; j > idx + add_size; --j){
        data_m[j -1] = data_m[j - 1 - add_size];
    }
    for (int g = idx;g < idx + add_size;++g){
        data_m[g] = cstr[g - idx];
    }
    data_m[size_m] = '\0';
};

//Nastia Shvets
void my_str_t::append(const my_str_t& str) {
    size_t add_size = str.size();
    if (size_m + add_size > capacity_m) {
        capacity_m  = calculate_capacity(size_m+add_size);
    }

    char *new_data = new char[capacity_m + 1];
    for (int i = 0; i < size_m; i++) {
        new_data[i] = data_m[i];
    }
    for (int i = 0; i < add_size; i++) {
        new_data[size_m + i] = str[i];
    }
    size_m = size_m + add_size;
    new_data[size_m] = '\0';
    delete[] data_m;
    data_m = new_data;
}

//Nastia Shvets
void my_str_t::append(char c) {
    size_m += 1;
    if (size_m > capacity_m) {
        capacity_m = calculate_capacity(size_m);
        char *new_data = new char[capacity_m + 1];
        for (int i = 0; i < size_m - 1; i++) {
            new_data[i] = data_m[i];
        }
        new_data[size_m - 1] = c;
        delete [] data_m;
        data_m = new_data;
    }
    else {
        data_m[size_m - 1] = c;
    }
    data_m[size_m] = '\0';
}

//Made by: Sofia Knyshoyid
void my_str_t::append(const char* cstr){
    size_t add_size = strlen(cstr);
    if (size_m + add_size > capacity_m) {
        capacity_m  = calculate_capacity(size_m+add_size);
    }

    char *new_data = new char[capacity_m + 1];
    for (int i = 0; i < size_m; i++) {
        new_data[i] = data_m[i];
    }
    for (int i = 0; i < add_size; i++) {
        new_data[size_m + i] = cstr[i];
    }
    size_m = size_m + add_size;
    new_data[size_m] = '\0';
    delete[] data_m;
    data_m = new_data;
};

//Made by: Sofia Knyshoyid
void my_str_t::erase(size_t begin , size_t size) {
    if (size_m < begin) {
        throw std::out_of_range("Index is out of range");
    }
    if (begin + size > size_m) {
        size = size_m - begin;
    }

    char *new_data = new char[capacity_m + 1];

    for (int i = 0; i < begin; i++) {
        new_data[i] = data_m[i];
    }
    for (int i = begin + size; i < size_m; ++i) {
            new_data[i - size] = data_m[i];
    }

    size_m = size_m - size;
    new_data[size_m] = '\0';
    delete[] data_m;
    data_m = new_data;
}

//Made by: Kazymyr Arsenii
size_t my_str_t::size() const noexcept {
    return size_m;
}
size_t my_str_t::capacity() const noexcept {
    return capacity_m;
}

//Nastia Shvets
const char* my_str_t::c_str() const {
    return data_m;
}

static constexpr size_t not_found = -1;

//Yaryna Fialko
size_t my_str_t::find(char c, size_t idx) {
    if (idx > size_m){throw std::out_of_range("Index is out of range");}
    for (size_t i = idx; i < size_m; ++i){
        if (data_m[i] == c) {return i;}
    }
    return my_str_t::not_found;
}

//Yaryna Fialko
size_t my_str_t::find(const std :: string& str , size_t idx){
    if (idx > size_m){throw std::out_of_range("Index is out of range");}
    bool notBroken = true;
    for (size_t i = idx; i < size_m; ++i){
        for (int j = 0; j < str.size(); ++j){
            if (data_m[i+j] != str[j]){notBroken = false; break;}
        }
        if (notBroken){return i;}
        notBroken= true;
    }
    return my_str_t::not_found;
}

//Yaryna Fialko
size_t my_str_t::find(const char* cstr, size_t idx){
    size_t cstr_len = strlen(cstr);
    if (idx > size_m){throw std::out_of_range("Index is out of range");}
    bool notBroken = true;
    for (size_t i = idx; i < size_m; ++i){
        for (int j = 0; j < cstr_len; ++j){
            if (data_m[i+j] != cstr[j]){notBroken = false; break;}
        }
        if (notBroken){return i;}
        notBroken= true;
    }
    return my_str_t::not_found;
}

// Kazymyr Arsenii
my_str_t my_str_t::substr(size_t begin, size_t size) {
    if (begin>size_m) {
        throw std::out_of_range("Index is out of range");
    }
    my_str_t output = my_str_t(size, '0');
    size_t j = 0;
    for (size_t i = begin; i < begin + size; i++, j++) {
        output[j] = data_m[i%size_m];
    }
    return output;
}

//Nastia Shvets
my_str_t::~my_str_t(){
    delete[] data_m;
}
std::ostream& operator<<(std::ostream& stream, const my_str_t& str){
    stream << str.c_str();
    return stream;
}
//Made by: Sofia Knyshoyid
std::istream& operator>>(std::istream& stream, my_str_t& str){
    while (!isspace(stream.peek()) && stream){
        str.append(stream.get());
    return stream;   
    }
}
//Made by: Sofia Knyshoyid
std::istream& readline(std::istream& stream, my_str_t& str){
    while (stream.peek()!='\n' && stream){
        str.append(stream.get());
    return stream;   
    }
}


//Yaryna Fialko
bool operator==(const my_str_t &str1, const my_str_t &str2) {
    if (str1.size() == str2.size()){
        for (size_t i = 0; i < str1.size(); ++i) {
            if (str1[i] != str2[i]){return  false;}}
        return true;
    }
    return false;
}

//Yaryna Fialko
bool operator !=(const my_str_t& str1, const my_str_t& str2){
    return  !(str1 == str2);
}

//Yaryna Fialko
bool operator> (const my_str_t& str1, const my_str_t& str2) {
    for (size_t i = 0; i < str1.size() && i < str2.size(); ++i) {
        if (str1[i] != str2[i]){return str1[i] > str2[i];}
    }
    return str1.size() > str2.size();
}

//Yaryna Fialko
bool operator>=(const my_str_t &str1, const my_str_t &str2) {
    return !(str1 < str2);
}

//Yaryna Fialko
bool operator<(const my_str_t &str1, const my_str_t &str2) {
    for (size_t i = 0; i < str1.size() && i < str2.size(); ++i) {
        if (str1[i] != str2[i]) {return str1[i] < str2[i];}
    }
    return str1.size() < str2.size();
}
//Yaryna Fialko
bool operator<=(const my_str_t &str1, const my_str_t &str2) {
    return !(str1 > str2);
}

//Yaryna Fialko
bool operator==(const my_str_t &str1, const char *cstr2) {
    size_t cstr_len = my_str_t::strlen(cstr2);
    if (str1.size() == cstr_len) {
        for (size_t i = 0; i < str1.size(); ++i) {
            if (str1[i] != cstr2[i]) {return false;}
            }
            return true;
        }
    return false;
}

//Yaryna Fialko
bool operator!=(const my_str_t &str1, const char *cstr2) {
    return !(str1 == cstr2);
}

//Yaryna Fialko
bool operator>(const my_str_t &str1, const char *cstr2) {
    size_t cstr_len = my_str_t::strlen(cstr2);
    for (size_t i = 0; i < str1.size() && i < cstr_len; ++i) {
        if (str1[i] != cstr2[i]) { return str1[i] > cstr2[i]; }
    }
    return str1.size() > cstr_len;

}
//Yaryna Fialko
bool operator>=(const my_str_t &str1, const char *cstr2) {
    return !(str1 < cstr2);
}

//Yaryna Fialko
bool operator<(const my_str_t &str1, const char *cstr2) {
    size_t cstr_len = my_str_t::strlen(cstr2);
    for (size_t i = 0; i < str1.size() && i < cstr_len; ++i) {
        if (str1[i] != cstr2[i]) { return str1[i] < cstr2[i]; }
    }
    return str1.size() < cstr_len;
}

//Yaryna Fialko
bool operator<=(const my_str_t &str1, const char *cstr2) {
    return !(str1 > cstr2);
}

//Yaryna Fialko
bool operator==(const char* cstr1, const my_str_t& str2){
    size_t cstr_len = my_str_t::strlen(cstr1);
    if (str2.size() == cstr_len) {
        for (size_t i = 0; i < str2.size(); ++i) {
            if (cstr1[i] != str2[i]) {return false;}}
        return true;
    }
    return false;
}

//Yaryna Fialko
bool operator !=(const char* cstr1, const my_str_t& str2){
    return !(cstr1 == str2);
}

//Yaryna Fialko
bool operator> (const char* cstr1, const my_str_t& str2){
    size_t cstr_len = my_str_t::strlen(cstr1);
    for (size_t i = 0; i < str2.size() && i < cstr_len; ++i) {
        if (cstr1[i] != str2[i]) { return  cstr1[i] > str2[i]; }
    }
    return cstr_len > str2.size();
}

//Yaryna Fialko
bool operator>=(const char* cstr1, const my_str_t& str2){
    return !(cstr1 < str2);
}

//Yaryna Fialko
bool operator< (const char* cstr1, const my_str_t& str2){
    size_t cstr_len = my_str_t::strlen(cstr1);
    for (size_t i = 0; i < str2.size() && i < cstr_len; ++i) {
        if (cstr1[i] != str2[i]) { return  cstr1[i] < str2[i]; }
    }
    return cstr_len < str2.size();
}

//Yaryna Fialko
bool operator<=(const char* cstr1, const my_str_t& str2){
    return !(cstr1 > str2);
}

//Nastia Shvets
void my_str_t::operator+=(const std::string& cstr) {
    size_t add_size = cstr.length();
    if (size_m + add_size > capacity_m) {
        capacity_m = calculate_capacity(size_m + add_size);
    }

    char* new_data = new char[capacity_m + 1];
    for (int i = 0; i < size_m; i++) {
        new_data[i] = data_m[i];
    }
    for (int i = 0; i < add_size; i++) {
        new_data[size_m + i] = cstr[i];
    }
    size_m = size_m + add_size;
    new_data[size_m] = '\0';
    delete[] data_m;
    data_m = new_data;
}

//Nastia Shvets
void my_str_t::operator+=(const char* cstr) {
    append(cstr);
}

void my_str_t::operator+=(char c) {
    append(c);
}

my_str_t operator+(const my_str_t& str1, const std::string& cstr) {
    my_str_t new_str = str1;
    new_str += cstr;
    return new_str;
}

my_str_t operator+(const std::string& cstr, const my_str_t& str1) {
    my_str_t output = str1;
    output.insert(0, cstr.c_str());
    return output;
}

my_str_t operator+(const my_str_t& str1, const char* cstr) {
    my_str_t new_str = str1;
    new_str += cstr;
    return new_str;
}

my_str_t operator+(const char* cstr, const my_str_t& str1) {
    my_str_t output = str1;
    output.insert(0, cstr);
    return output;
}

my_str_t operator+(const my_str_t& str1, char c) {
    my_str_t new_str = str1;
    new_str += c;
    return new_str;
}

// Kazymyr Arsenii
my_str_t operator+(const my_str_t& str1, const my_str_t& str2) {
    my_str_t output = str1;
    output.append(str2);
    return output;
}

void my_str_t::operator+=(const my_str_t& str) {
    size_t add_size = str.size();
    if (size_m + add_size > capacity_m) {
        capacity_m = calculate_capacity(size_m + add_size);
    }

    char* new_data = new char[capacity_m + 1];
    for (int i = 0; i < size_m; i++) {
        new_data[i] = data_m[i];
    }
    for (int i = 0; i < add_size; i++) {
        new_data[size_m + i] = str[i];
    }
    size_m = size_m + add_size;
    new_data[size_m] = '\0';
    delete[] data_m;
    data_m = new_data;
}

void my_str_t::operator*=(const size_t num) {
    if (size_m * num > capacity_m) {
        capacity_m = calculate_capacity(size_m * num);
    }
    char* new_data = new char[capacity_m + 1];
    for (size_t i = 0; i < size_m * num; ++i) {
        new_data[i] = data_m[i % size_m];
    }
    size_m *= num;
    new_data[size_m] = '\0';
    delete[] data_m;
    data_m = new_data;
}

my_str_t operator*(const size_t num, const my_str_t & str){
    my_str_t output = str;
    output *= num;
    return output;
}

my_str_t operator*(const my_str_t & str, const size_t num){
    return num*str;
}
