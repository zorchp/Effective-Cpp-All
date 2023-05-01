#include <set>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <iterator>


using namespace std;

struct StringSize {
    string::size_type operator()(const string& s) const { return s.size(); }
};

void t1() {
    set<string> s{"abc", "ccccc", "b"};
    transform(s.begin(), s.end(),
              ostream_iterator<string::size_type>(cout, " "),
              mem_fn(&string::size)); // 3 1 5
    // 或者使用函数对象
    transform(s.begin(), s.end(),
              ostream_iterator<string::size_type>(cout, " "),
              StringSize()); // 3 1 5
}

template <typename T>
T average(T val1, T val2) {
    return (val1 + val2) / 2;
}


template <typename T1, typename T2>
void writeAverage(T1 beg1, T1 end1, T2 beg2, ostream& s) {
    transform(
        beg1, end1, beg2,
        ostream_iterator<typename iterator_traits<T1>::value_type>(s, " "),
        average<typename iterator_traits<T1>::value_type>);
}


void t2() {
    //
    vector<int> v1{1, 2, 3, 4, 5}, v2{6, 7, 8, 9, 10, 11, 12};
    // 两个序列对应取平均值, 但是可能存在二义性问题
    // 如果还有一个average函数模板也是只有一个模板类型, 则ambiguous
    writeAverage(v1.begin(), v1.end(), v2.begin(), cout);
    // 3 4 5 6 7
}

template <typename T>
struct Average {
    T operator()(T v1, T v2) const { return average(v1, v2); }
};

template <typename T1, typename T2>
void writeAverage1(T1 beg1, T1 end1, T2 beg2, ostream& s) {
    transform(
        beg1, end1, beg2,
        ostream_iterator<typename iterator_traits<T1>::value_type>(s, " "),
        // average<typename iterator_traits<T1>::value_type>);
        Average<typename iterator_traits<T1>::value_type>());
}

void t3() {
    //
    vector<int> v1{1, 2, 3, 4, 5}, v2{6, 7, 8, 9, 10, 11, 12};
    // 两个序列对应取平均值, 但是可能存在二义性问题
    // 如果还有一个average函数模板也是只有一个模板类型, 则ambiguous
    writeAverage1(v1.begin(), v1.end(), v2.begin(), cout);
    // 3 4 5 6 7
}

int main(int argc, char const* argv[]) {
    // t1();
    // t2();
    t3();
    return 0;
}