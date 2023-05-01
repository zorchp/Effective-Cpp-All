#include <string>
#include <vector>
#include <iostream>
using namespace std;


void t1() {
    string s1{};
    cout << sizeof(s1) << endl;
    // clang++: 24
    // g++: 32
}
void t2() {
    string s2("abc");
    s2.push_back('\0');
    s2.push_back('a');
    s2.push_back('b');
    cout << s2 << endl; // abcab
    const char* c_s2 = s2.c_str();
    cout << c_s2 << endl; // abc
}
void t3() {
    string s2("abc\0abc"s);
    cout << s2 << endl; // abcabc
    const char* c_s2 = s2.c_str();
    cout << c_s2 << endl; // abc
}

int main(int argc, char const* argv[]) {
    // t1();
    // t2();
    t3();
    return 0;
}