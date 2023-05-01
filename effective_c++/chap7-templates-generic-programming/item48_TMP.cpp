#include <iostream>

using namespace std;

/* 模版元编程 */
// 例1: 编译期实现递归计算阶乘
template <unsigned n>
struct Factorial {
    enum { value = n * Factorial<n - 1>::value };
};

template <>
struct Factorial<0> {
    enum { value = 1 };
};

void t1() {
    cout << Factorial<10>::value << endl; // 3628800
}

int main(int argc, char *argv[]) {
    t1();
    return 0;
}
