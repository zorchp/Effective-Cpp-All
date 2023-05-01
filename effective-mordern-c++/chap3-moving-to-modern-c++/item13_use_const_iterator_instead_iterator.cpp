#include <iterator>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

void t1() {
    // 应该使用const_iterator
    vector<int> vals;
    vector<int>::iterator it = find(vals.begin(), vals.end(), 1903);
    vals.insert(it, 1990);
}
typedef vector<int>::iterator IterT;
typedef vector<int>::const_iterator ConstIterT;

void t2() {
    // For C++98

    vector<int> vals;
    ConstIterT ci = find(static_cast<ConstIterT>(vals.begin()),
                         static_cast<ConstIterT>(vals.end()), 1982);
    vals.insert(static_cast<ConstIterT>(ci), 1998);
}

void t3() {
    // c++11
    vector<int> vals;
    ConstIterT ci = find(vals.cbegin(), vals.cend(), 1900);
    vals.insert(ci, 1998);
}

// 一个可能的全局函数cbegin()实现
template <class C>
auto cbegin1(const C &container) -> decltype(begin(container)) {
    return begin(container);
}

// 使用C++14的decltype(auto)
template <class C>
decltype(auto) cbegin2(const C &container) {
    return begin(container);
}

void t4() {
    // 用这种方法, 可以针对C-style数组生成常量引用
    int a[10]{};
    auto aa = cbegin2(a);
    cout << typeid(aa).name() << endl; // PKi
}

int main(int argc, char *argv[]) {
    /* t2(); */
    t4();
    return 0;
}
