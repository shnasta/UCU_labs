#include "mystring.h"

void run_constructors() {
    std::cout<<"my_str_t(100, 'w'):\n";
    my_str_t str = my_str_t(100, 'w');
    std::cout << "  str = " << str << std::endl;
    std::cout << "  capacity = " << str.capacity() << std::endl;
    std::cout << "  size = " << str.size() << '\n';
    std::cout << '\n';

    std::cout<<"my_str_t(char* \"ggg\"):\n";
    char some_chars[]  = "ggg";
    my_str_t char_str = my_str_t(some_chars);
    std::cout << "  str = " << char_str << std::endl;
    std::cout << "  capacity = " << char_str.capacity() << std::endl;
    std::cout << "  size = " << char_str.size() << '\n';
    std::cout << '\n';

    std::cout<<"my_str_t(std::string& = \"hello world\"):\n";
    std::string some_string =  "hello world";
    my_str_t string_str = my_str_t(some_string);
    std::cout << "  str = " << string_str << std::endl;
    std::cout << "  capacity = " << string_str.capacity() << std::endl;
    std::cout << "  size = " << string_str.size() << '\n';
    std::cout << '\n';

    std::cout<<"my_str_t(my_str_t& = my_str_t(\"hello my_str_t\")):\n";
    my_str_t some_str =  my_str_t("hello my_str_t");
    my_str_t new_str = my_str_t(some_str);
    std::cout << "  str = " << new_str << std::endl;
    std::cout << "  capacity = " << new_str.capacity() << std::endl;
    std::cout << "  size = " << new_str.size() << '\n';
    std::cout << '\n';
}

void run_strlen_and_swap() {
    std::cout<<"strlen - returns size of c sting type"<<'\n';
    std::cout<<"    strlen(\"abcde\") = "<<my_str_t::strlen("abcde")<<'\n';
    std::cout<<'\n';

    std::cout<<"swap(my_str_t& other):\n";
    my_str_t first_str =  my_str_t("hello from first_str");
    my_str_t second_str = my_str_t("hello from second_str");
    std::cout << "  first_str = " << first_str << std::endl;
    std::cout << "  second_str = " << second_str << std::endl;
    first_str.swap(second_str);
    std::cout << "after swapping:" << std::endl;
    std::cout << "  first_str = " << first_str << std::endl;
    std::cout << "  second_str = " << second_str << std::endl;
    std::cout << '\n';
}

void run_index() {
    std::cout << "operator [ ] and at() :" << '\n';
    my_str_t example = my_str_t("abcdefghij");
    std::cout << "  example = " << example << std::endl;
    std::cout << "  example[5] = " << example[5] << std::endl;
    std::cout << "  example[33] = " << example[33] << std::endl;
    std::cout << "  example.at(5) = " << example.at(5) << std::endl;
    std::cout << "  example.at(9) = " << example.at(9) << std::endl;
    std::cout << '\n';

    std::cout << "operator [ ] and at() const :" << '\n';
    const my_str_t example1 = my_str_t("1abcdefghij");
    std::cout << "  example1 = " << example1 << std::endl;
    std::cout << "  example1[5] = " << example1[5] << std::endl;
    std::cout << "  example1[33] = " << example1[33] << std::endl;
    std::cout << "  example.at(5) = " << example1.at(5) << std::endl;
    std::cout << "  example.at(10) = " << example1.at(10) << std::endl;
    std::cout << '\n';
}

void run_reserve_and_shrink_to_fit(my_str_t reserve_str) {
    std::cout<<"reserve(new_capacity):\n";
    std::cout << "  str = " << reserve_str << std::endl;
    std::cout << "  capacity = " << reserve_str.capacity() << std::endl;
    std::cout << "  size = " << reserve_str.size() << std::endl;
    reserve_str.reserve(33);
    std::cout << "after reserve(33)" << std::endl;
    std::cout << "  str = " << reserve_str << std::endl;
    std::cout << "  capacity = " << reserve_str.capacity() << std::endl;
    std::cout << "  size = " << reserve_str.size() << std::endl;
    std::cout << "then after reserve(15)" << std::endl;
    std::cout << "  str = " << reserve_str << std::endl;
    std::cout << "  capacity = " << reserve_str.capacity() << std::endl;
    std::cout << "  size = " << reserve_str.size() << std::endl;
    std::cout << '\n';


    std::cout << "previous str after shrink_to_fit()" << std::endl;
    reserve_str.shrink_to_fit();
    std::cout << "  str = " << reserve_str << std::endl;
    std::cout << "  capacity = " << reserve_str.capacity() << std::endl;
    std::cout << "  size = " << reserve_str.size() << std::endl;
    std::cout << '\n';
}

void run_resize(my_str_t reserve_str) {
    std::cout << "previous str resize(50, 'b')" << std::endl;
    reserve_str.resize(50, 'b');
    std::cout << "  str = " << reserve_str << std::endl;
    std::cout << "  capacity = " << reserve_str.capacity() << std::endl;
    std::cout << "  size = " << reserve_str.size() << std::endl;
    std::cout << "previous str resize(18)" << std::endl;
    reserve_str.resize(53);
    std::cout << "  str = " << reserve_str << std::endl;
    std::cout << "  capacity = " << reserve_str.capacity() << std::endl;
    std::cout << "  size = " << reserve_str.size() << std::endl;
    std::cout << '\n';
}

void run_appends(my_str_t reserve_str) {
    std::cout << "previous str append('b')" << std::endl;
    reserve_str.append('b');
    std::cout << "  str = " << reserve_str << std::endl;
    std::cout << "  capacity = " << reserve_str.capacity() << std::endl;
    std::cout << "  size = " << reserve_str.size() << std::endl;
    std::cout << "previous str append(my_str_t(\" i am here\"))" << std::endl;
    reserve_str.append(my_str_t(" i am here"));
    std::cout << "  str = " << reserve_str << std::endl;
    std::cout << "  capacity = " << reserve_str.capacity() << std::endl;
    std::cout << "  size = " << reserve_str.size() << std::endl;
    std::cout << "previous str append(\", i am here again!\")" << std::endl;
    reserve_str.append(", i am here again!");
    std::cout << "  str = " << reserve_str << std::endl;
    std::cout << "  capacity = " << reserve_str.capacity() << std::endl;
    std::cout << "  size = " << reserve_str.size() << std::endl;
    std::cout << '\n';
}

void run_clear(my_str_t reserve_str) {
    std::cout << "previous str clear()" << std::endl;
    reserve_str.clear();
    std::cout << "  str = " << reserve_str << std::endl;
    std::cout << "  capacity = " << reserve_str.capacity() << std::endl;
    std::cout << "  size = " << reserve_str.size() << std::endl;
    std::cout << '\n';
}

void run_finds() {
    std::cout << '\n';
    std::cout << "find(char c, size_t idx):" << std::endl;
    my_str_t str_found = my_str_t("i am looking for 'a'");
    std::cout << "  str_found = " << str_found << std::endl;
    std::cout << "  str_found.find('a', 5) = " << str_found.find('a', 5) << std::endl;
    std::cout << '\n';
    my_str_t str_found1 = my_str_t("i am looking for 'b'");
    std::cout << "  str_found = " << str_found1 << std::endl;
    std::cout << "  str_found.find('b', 19) = " << str_found1.find('b', 19) << std::endl;
    std::cout << '\n';

    std::cout << "find(const std::string& str , size_t idx):" << std::endl;
    std::string find_string = "find me";
    my_str_t std_str_found = my_str_t("i am looking for 'find me'");
    std::cout << "string = " << find_string << std::endl;
    std::cout << "  str_found = " << std_str_found << std::endl;
    std::cout << "  str_found.find(string, 5) = " << std_str_found.find(find_string, 5) << std::endl;
    std::cout << '\n';
    my_str_t std_str_found1 = my_str_t("try to find me here");
    std::cout << "  str_found = " << std_str_found1 << std::endl;
    std::cout << "  str_found.find(string, 8) = " << std_str_found1.find(find_string, 8) << std::endl;
    std::cout << '\n';

    std::cout << "find(const char* cstr, size_t idx):" << std::endl;
    char find_chars[] = "some chars";
    my_str_t chars_found = my_str_t("i am looking for 'some chars'");
    std::cout << "chars = " << find_chars << std::endl;
    std::cout << "  str_found = " << chars_found << std::endl;
    std::cout << "  str_found.find(chars, 19) = " << chars_found.find(find_chars, 19) << std::endl;
    std::cout << '\n';
    my_str_t chars_found1 = my_str_t("you can find some chars at 13");
    std::cout << "  str_found = " << chars_found1 << std::endl;
    std::cout << "  str_found.find(chars, 5) = " << chars_found1.find(find_chars, 5) << std::endl;
    std::cout << '\n';
    std::cout << '\n';
}

void run_inserts() {
    std::cout << '\n';
    std::cout<<"insert(size_t idx , char c):\n";
    my_str_t insertion_str = my_str_t(14, 'a');
    std::cout << "  str = " <<  insertion_str << std::endl;
    std::cout << "  capacity = " << insertion_str.capacity() << std::endl;
    std::cout << "  size = " << insertion_str.size() << std::endl;
    insertion_str.insert(5, 'b');
    std::cout << "after insert(5, 'b')" << std::endl;
    std::cout << "  str = " <<  insertion_str << std::endl;
    std::cout << "  capacity = " << insertion_str.capacity() << std::endl;
    std::cout << "  size = " << insertion_str.size() << std::endl;
    insertion_str.insert(5, 'b');
    std::cout << "after insert(5, 'b')" << std::endl;
    std::cout << "  str = " <<  insertion_str << std::endl;
    std::cout << "  capacity = " << insertion_str.capacity() << std::endl;
    std::cout << "  size = " << insertion_str.size() << std::endl;
    std::cout << '\n';

    std::cout<<"insert(size_t idx , const my_str_t& str):\n";
    my_str_t new_insert = my_str_t(4, 'a');
    std::cout << "  str = " <<  new_insert << std::endl;
    std::cout << "  capacity = " << new_insert.capacity() << std::endl;
    std::cout << "  size = " << new_insert.size() << std::endl;
    new_insert.insert(0, my_str_t("i am here"));
    std::cout << "after insert(0, my_str_t(\"i am here\"))" << std::endl;
    std::cout << "  str = " <<  new_insert << std::endl;
    std::cout << "  capacity = " << new_insert.capacity() << std::endl;
    std::cout << "  size = " << new_insert.size() << std::endl;
    new_insert.insert(9, my_str_t(", another loong try"));
    std::cout << "after insert(9, my_str_t(\", another loong try\"))" << std::endl;
    std::cout << "  str = " <<  new_insert << std::endl;
    std::cout << "  capacity = " <<new_insert.capacity() << std::endl;
    std::cout << "  size = " << new_insert.size() << std::endl;
    std::cout << '\n';

    std::cout<<"insert(size_t idx , const char* cstr):\n";
    my_str_t another_insert = my_str_t(8, 'g');
    std::cout << "  str = " <<  another_insert << std::endl;
    std::cout << "  capacity = " << another_insert.capacity() << std::endl;
    std::cout << "  size = " << another_insert.size() << std::endl;
    another_insert.insert(0, "i am here");
    std::cout << "after insert(0,{\"i am here\"})" << std::endl;
    std::cout << "  str = " <<  another_insert << std::endl;
    std::cout << "  capacity = " << another_insert.capacity() << std::endl;
    std::cout << "  size = " << another_insert.size() << std::endl;
    another_insert.insert(9, ", another loong try");
    std::cout << "after insert(9, {\", another loong try\"})" << std::endl;
    std::cout << "  str = " <<  another_insert << std::endl;
    std::cout << "  capacity = " <<another_insert.capacity() << std::endl;
    std::cout << "  size = " << another_insert.size() << std::endl;
    std::cout << '\n';
    std::cout << '\n';
}

void run_erase() {
    std::cout << "erase(size_t begin , size_t size):" << std::endl;
    my_str_t erasing_str = my_str_t("abcdefghijklmnopqrstuvwxyz");
    std::cout << "  str = " <<  erasing_str << std::endl;
    std::cout << "  capacity = " << erasing_str.capacity() << std::endl;
    std::cout << "  size = " << erasing_str.size() << std::endl;
    erasing_str.erase(5,11);
    std::cout << "after erase(5, 11)" << std::endl;
    std::cout << "  str = " <<  erasing_str << std::endl;
    std::cout << "  capacity = " << erasing_str.capacity() << std::endl;
    std::cout << "  size = " << erasing_str.size() << std::endl;
    erasing_str.erase(5,11);
    std::cout << "after erase(5, 11) again" << std::endl;
    std::cout << "  str = " <<  erasing_str << std::endl;
    std::cout << "  capacity = " << erasing_str.capacity() << std::endl;
    std::cout << "  size = " << erasing_str.size() << std::endl;
    std::cout << '\n';
}

void run_substr() {
    std::cout << "substr(size_t begin, size_t size):" << std::endl;
    my_str_t parent_str = my_str_t("abcdefghijklmnopqrstuvwxyz");
    std::cout << "  str = " <<  parent_str << std::endl;
    std::cout << "  capacity = " << parent_str.capacity() << std::endl;
    std::cout << "  size = " << parent_str.size() << std::endl;
    my_str_t child_str = parent_str.substr(5, 11);
    std::cout << "after str.substr(5, 11)" << std::endl;
    std::cout << "  copied_str = " <<  child_str << std::endl;
    std::cout << "  capacity = " << child_str.capacity() << std::endl;
    std::cout << "  size = " << child_str.size() << std::endl;
    std::cout << '\n';
}

void run_bool_operators() {
    my_str_t str1 = my_str_t("apple");
    my_str_t str2 = my_str_t("peach");
    my_str_t str3 = my_str_t("apple");
    my_str_t str4 = my_str_t("papaya");
    std::cout << "  str1 = " << str1 << std::endl;
    std::cout << "  str2 = " << str2 << std::endl;
    std::cout << "  str3 = " << str3 << std::endl;
    std::cout << "  str4 = " << str4 << std::endl;
    std::cout << '\n';

    std::cout << "operator==(const my_str_t& str1, const my_str_t& str2)"<<'\n';
    std::cout << "  str1 == str2 (" << str1 << " == " << str2 <<") : "<< (str1 == str2) << std::endl;
    std::cout << "  str1 == str3 (" << str1 << " == " << str3 <<") : "<< (str1 == str3) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator !=(const my_str_t& str1, const my_str_t& str2)"<<'\n';
    std::cout << "  str1 != str2 (" << str1 << " != " << str2 <<") : "<< (str1 != str2) << std::endl;
    std::cout << "  str1 != str3 (" << str1 << " != " << str3 <<") : "<< (str1 != str3) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator> (const my_str_t& str1, const my_str_t& str2)"<<'\n';
    std::cout << "  str2 > str1 (" << str2 << " > " << str1 <<") : "<< (str2 > str1) << std::endl;
    std::cout << "  str1 > str4 (" << str1 << " > " << str4 <<") : "<< (str1 > str4) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator>=(const my_str_t& str1, const my_str_t& str2)"<<'\n';
    std::cout << "  str2 >= str1 (" << str2 << " >= " << str1 <<") : "<< (str2 >= str1) << std::endl;
    std::cout << "  str1 >= str3 (" << str1 << " >= " << str3 <<") : "<< (str1 >= str3) << std::endl;
    std::cout << "  str1 >= str4 (" << str1 << " >= " << str4 <<") : "<< (str1 >= str4) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator< (const my_str_t& str1, const my_str_t& str2)"<<'\n';
    std::cout << "  str2 < str1 (" << str2 << " < " << str1 <<") : "<< (str2 < str1) << std::endl;
    std::cout << "  str1 < str4 (" << str1 << " < " << str4 <<") : "<< (str1 < str4) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator<=(const my_str_t& str1, const my_str_t& str2)"<<'\n';
    std::cout << "  str2 <= str1 (" << str2 << " <= " << str1 <<") : "<< (str2 <= str1) << std::endl;
    std::cout << "  str1 <= str3 (" << str1 << " <= " << str3 <<") : "<< (str1 <= str3) << std::endl;
    std::cout << "  str1 <= str4 (" << str1 << " <= " << str4 <<") : "<< (str1 <= str4) << std::endl;
    std::cout << '\n';

    std::string cstr1 = "apple";
    std::string cstr2 = "peach";
    std::string cstr3 = "apple";
    std::string cstr4 = "papaya";
    std::cout << "  cstr1 = " << cstr1 << std::endl;
    std::cout << "  cstr2 = " << cstr2 << std::endl;
    std::cout << "  cstr3 = " << cstr3 << std::endl;
    std::cout << "  cstr4 = " << cstr4 << std::endl;
    std::cout << '\n';

    std::cout << "bool operator==(const my_str_t& str1, const char* cstr2 )"<<'\n';
    std::cout << "  str1 == cstr2 (" << str1 << " == " << cstr2 <<") : "<< (str1 == cstr2) << std::endl;
    std::cout << "  str1 == cstr3 (" << str1 << " == " << cstr3 <<") : "<< (str1 == cstr3) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator !=( const my_str_t& str1, const char* cstr2 )"<<'\n';
    std::cout << "  str1 != cstr2 (" << str1 << " != " << cstr2 <<") : "<< (str1 != cstr2) << std::endl;
    std::cout << "  str1 != cstr3 (" << str1 << " != " << cstr3 <<") : "<< (str1 != cstr3) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator> ( const my_str_t& str1, const char* cstr2 )"<<'\n';
    std::cout << "  str2 > cstr1 (" << str2 << " > " << cstr1 <<") : "<< (str2 > cstr1) << std::endl;
    std::cout << "  str1 > cstr4 (" << str1 << " > " << cstr4 <<") : "<< (str1 > cstr4) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator>=(const my_str_t& str1, const char* cstr2 )"<<'\n';
    std::cout << "  str2 >= cstr1 (" << str2 << " >= " << cstr1 <<") : "<< (str2 >= cstr1) << std::endl;
    std::cout << "  str1 >= cstr3 (" << str1 << " >= " << cstr3 <<") : "<< (str1 >= cstr3) << std::endl;
    std::cout << "  str1 >= cstr4 (" << str1 << " >= " << cstr4 <<") : "<< (str1 >= cstr4) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator< ( const my_str_t& str1, const char* cstr2 )"<<'\n';
    std::cout << "  str2 < str1 (" << str2 << " < " << cstr1 <<") : "<< (str2 < cstr1) << std::endl;
    std::cout << "  str1 < str4 (" << str1 << " < " << cstr4 <<") : "<< (str1 < cstr4) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator<=(const my_str_t& str1, const char* cstr2 )"<<'\n';
    std::cout << "  str2 <= cstr1 (" << str2 << " <= " << cstr1 <<") : "<< (str2 <= cstr1) << std::endl;
    std::cout << "  str1 <= cstr3 (" << str1 << " <= " << cstr3 <<") : "<< (str1 <= cstr3) << std::endl;
    std::cout << "  str1 <= cstr4 (" << str1 << " <= " << cstr4 <<") : "<< (str1 <= cstr4) << std::endl;
    std::cout << '\n';


    std::cout << "bool operator==(const char* cstr1, const my_str_t& str2)"<<'\n';
    std::cout << "  cstr1 == str2 (" << cstr1 << " == " << str2 <<") : "<< (cstr1 == str2) << std::endl;
    std::cout << "  cstr1 == str3 (" << cstr1 << " == " << str3 <<") : "<< (cstr1 == str3) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator !=(const char* cstr1, const my_str_t& str2)"<<'\n';
    std::cout << "  cstr1 != str2 (" << cstr1 << " != " << str2 <<") : "<< (cstr1 != str2) << std::endl;
    std::cout << "  cstr1 != str3 (" << cstr1 << " != " << str3 <<") : "<< (cstr1 != str3) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator> (const char* cstr1, const my_str_t& str2)"<<'\n';
    std::cout << "  cstr2 > str1 (" << cstr2 << " > " << str1 <<") : "<< (cstr2 > str1) << std::endl;
    std::cout << "  cstr1 > str4 (" << cstr1 << " > " << str4 <<") : "<< (cstr1 > str4) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator>=(const char* cstr1, const my_str_t& str2)"<<'\n';
    std::cout << "  cstr2 >= str1 (" << cstr2 << " >= " << str1 <<") : "<< (cstr2 >= str1) << std::endl;
    std::cout << "  cstr1 >= str3 (" << cstr1 << " >= " << str3 <<") : "<< (cstr1 >= str3) << std::endl;
    std::cout << "  cstr1 >= str4 (" << cstr1 << " >= " << str4 <<") : "<< (cstr1 >= str4) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator< (const char* cstr1, const my_str_t& str2)"<<'\n';
    std::cout << "  cstr2 < str1 (" << str2 << " < " << cstr1 <<") : "<< (cstr2 < str1) << std::endl;
    std::cout << "  cstr1 < str4 (" << str1 << " < " << cstr4 <<") : "<< (cstr1 < str4) << std::endl;
    std::cout << '\n';

    std::cout << "bool operator<=(const char* cstr1, const my_str_t& str2)"<<'\n';
    std::cout << "  cstr2 <= str1 (" << cstr2 << " <= " << str1 <<") : "<< (cstr2 <= str1) << std::endl;
    std::cout << "  cstr1 <= str3 (" << cstr1 << " <= " << str3 <<") : "<< (cstr1 <= str3) << std::endl;
    std::cout << "  cstr1 <= str4 (" << cstr1 << " <= " << str4 <<") : "<< (cstr1 <= str4) << std::endl;
    std::cout << '\n';
}

void run_concat_operators() {
    my_str_t my_str1 = my_str_t("pine");
    my_str_t my_str2 = my_str_t("apple");

    std::cout << "operator+(const my_str_t& str1, const my_str_t& str2)"<<'\n';
    std::cout << "my_str1 = "<< my_str1 << std::endl;
    std::cout << "my_str2 = "<< my_str2 << std::endl;
    my_str_t ne_str = my_str1 + my_str2;
    std::cout << "  my_str1 + my_str2 = "<<  (my_str1 + my_str2) << std::endl;
    std::cout << "my_str1 + \"abcd\" = " << my_str1 + "abcd" << std::endl;
    std::cout << "\"abcd\" + my_str1 = " << "abcd" + my_str1 << std::endl;
    std::string std_string = "i like";
    std::cout << "  std_string = "<< std_string << std::endl;
    std::cout << "  my_str2 + std_string = " << my_str2 + std_string << std::endl;
    std::cout << "  std_string + my_str1 = " << std_string + my_str2 << std::endl;
    char c = 'c';
    std::cout << "char c = "<< c << std::endl;
    std::cout << "my_str2 + c = " << my_str2 + c << std::endl;
    std::cout << '\n';


    std::cout << "operator+=(const my_str_t& str)"<<'\n';
    std::cout << "my_str1 = "<< my_str1 << std::endl;
    std::cout << "my_str2 = "<< my_str2 << std::endl;
    my_str1 += my_str2;
    std::cout << "  my_str1 += my_str2"<< std::endl;
    std::cout << "  my_str1 = "<< my_str1 << std::endl;
    my_str1 += "abcd";
    std::cout << "my_str1 += \"abcd\"" << std::endl;
    std::cout << "my_str1 = "<< my_str1 << std::endl;
    my_str2 += std_string;
    std::cout << "  my_str2 += std_string"<< std::endl;
    std::cout << "  my_str2 = "<< my_str2 << std::endl;
    my_str2 += c;
    std::cout << "my_str2 += c" << std::endl;
    std::cout << "my_str2 = "<< my_str2 << std::endl;
    std::cout << '\n';
}

void run_multiplicate_op() {
    my_str_t my_str1 = my_str_t("apple");
    std::cout << "operator*=(const size_t num)"<<'\n';
    std::cout << "  my_str1 = "<< my_str1 << std::endl;
    my_str1 *= 3;
    std::cout << "  my_str1 *= 3"<< std::endl;
    std::cout << "  my_str1 = "<< my_str1 << std::endl;
    std::cout << '\n';

    std::cout << "operator*(const size_t num, const my_str_t & str)"<<'\n';
    my_str_t new_str1 = my_str_t("one");
    std::cout << "  str = "<< new_str1 << std::endl;
    size_t num = 10;
    my_str_t mult = num * new_str1;
    std::cout << "  new_str = 10 * str"  << std::endl;
    std::cout << "  new_str = " << mult << std::endl;
    my_str_t mult1 = new_str1 * num;
    std::cout << "  new_str = str * 10"  << std::endl;
    std::cout << "  new_str = " << mult1 << std::endl;
}

int main(int args, char* argv[]) {
    run_constructors();
    run_strlen_and_swap();
    run_index();

    my_str_t reserve_str = my_str_t(10, 'a');
    run_reserve_and_shrink_to_fit(reserve_str);
    run_resize(reserve_str);
    run_appends(reserve_str);
    run_clear(reserve_str);

    run_finds();
    run_inserts();
    run_erase();
    run_substr();
    run_bool_operators();
    run_concat_operators();
    run_multiplicate_op();
}
