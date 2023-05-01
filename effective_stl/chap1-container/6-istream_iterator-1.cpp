#include <iostream>
#include <list>
#include <fstream>
#include <iterator>

using namespace std;

template <typename T>
ostream &operator<<(ostream &os, const list<T> &v) {
    for (T i : v) os << i << " ";
    return os << endl;
}

void t1() {
    // not a good way
    ifstream dataFile("test.txt");
    list<int> data(istream_iterator<int>(dataFile), istream_iterator<int>());
    // warning: parentheses were disambiguated as a function declaration
    cout << data; // 1
}

void t2() {
    // maybe a good way
    ifstream dataFile("test.txt");
    list<int> data((istream_iterator<int>(dataFile)), istream_iterator<int>());
    cout << data; // 1 2 3 4 5
}

void t3() {
    //  a good way
    ifstream dataFile("test.txt");
    auto l1 =
        list<int>(istream_iterator<int>(dataFile), istream_iterator<int>());
    cout << l1; // 1 2 3 4 5
}

void t4() {
    // a good way
    ifstream dataFile("test.txt");
    istream_iterator<int> dataBegin(dataFile), dataEnd;
    list<int> data(dataBegin, dataEnd);
    cout << data; // 1 2 3 4 5
}

int main(int argc, char const *argv[]) {
    // t1();
    // t2();
    // t3();
    t4();
    return 0;
}
