#include <iostream>
#include <vector>
#include <typeinfo>

using namespace std;
// 要点
// 1. 模版型别推导中, 具有引用类型的实参会被当成非引用型别来处理, 引用性忽略
// 2. 对万能引用形参进行推导, 左值实参会进行特殊处理
// 3. 按值传递的形参推导中, 若实参型别中带有const或volatile, const或volatile忽略
// 4. 模版型别推导中, 数组或函数类型的实参会退化成指针,
// 除非被用来初始化引用(按值传递会退化)

/* 形如下面的调用方式
template <typename T>
void f(ParamType param);

f(expr);
// 从expr来推导T和ParamType的类型
*/

namespace case1 {
// 1. ParamType是指针或引用, 但不是万能引用
// 规则:
// a. 若expr具有引用类型, 先将引用类型, 先忽略引用部分
// b. 对expr类型和ParamType类型执行模式匹配, 决定T的类型

template <typename T>
void f(T& param) {
    cout << typeid(T).name() << endl;               //
    cout << typeid(decltype(param)).name() << endl; //
}
template <typename T>
void g(const T& param) {
    cout << typeid(T).name() << endl;               //
    cout << typeid(decltype(param)).name() << endl; //
}
template <typename T>
void h(T* param) {
    cout << typeid(T).name() << endl;               //
    cout << typeid(decltype(param)).name() << endl; //
}
} // namespace case1

void t1() {
    int x = 8;
    const int cx = x;
    const int& rx = x;
    using namespace case1;
    f(x);
    f(cx);
    f(rx);
}
void t2() {
    int x = 8;
    const int cx = x;
    const int& rx = x;
    using namespace case1;
    g(x);
    g(cx);
    g(rx);
}

void t3() {
    int x = 8;
    const int* px = &x;
    using namespace case1;
    h(&x); // Pi
    h(px); // PKi
}

namespace case2 {

// 2. 万能引用
template <typename T>
void f(T&& param) {
    cout << typeid(T).name() << endl;               //
    cout << typeid(decltype(param)).name() << endl; //
}

} // namespace case2

void t4() {
    int x = 8;
    const int cx = x;
    const int& rx = x;
    using namespace case2;
    f(x);
    f(cx);
    f(rx);
    f(27);
}

namespace case3 {
// 3. 不是指针或引用, 按值传递
template <typename T>
void f(T param) {
    cout << typeid(T).name() << endl;               //
    cout << typeid(decltype(param)).name() << endl; //
}
} // namespace case3


void t5() {
    int x = 8;
    const int cx = x;
    const int& rx = x;
    using namespace case3;
    f(x);
    f(cx);
    f(rx);
    f(27);
}

namespace case_array {
template <typename T>
void f(T param) {
    cout << typeid(T).name() << endl;               //
    cout << typeid(decltype(param)).name() << endl; //
}
template <typename T>
void g(T& param) {
    cout << typeid(T).name() << endl;               //
    cout << typeid(decltype(param)).name() << endl; //
}

template <typename T, size_t N>
constexpr size_t arraySize(T (&)[N]) noexcept {
    return N;
}

} // namespace case_array

void t6() {
    const char name[] = "nihao";
    const char* ptrName = name; // 数组退化为首地址指针
    using namespace case_array;

    f(name);    // PKc
    f(ptrName); // PKc
    cout << "-------------------------------\n";
    g(name); // A6_c, 这是按引用传递的情况, 保留了数组的size信息
    g(ptrName);                      // PKc
    cout << arraySize(name) << endl; // 6
    int maps[arraySize(name)];       // 可以由此创建数组
}

namespace case_func_args {
void someFunc(int a, double b) {}
template <typename T>
void f(T param) {
    cout << typeid(T).name() << endl;               //
    cout << typeid(decltype(param)).name() << endl; //
}
template <typename T>
void g(T& param) {
    cout << typeid(T).name() << endl;               //
    cout << typeid(decltype(param)).name() << endl; //
}

} // namespace case_func_args

void t7() {
    using namespace case_func_args;
    f(someFunc); // FPFvid
    g(someFunc); // FvidE

    /* $ c++filt -t FvidE */
    /* void (int, double) */
    /* $ c++filt -t PFvidE */
    /* void (*)(int, double) */
}

int main(int argc, char* argv[]) {
    /* t1(); */
    /* t2(); */
    /* t3(); */
    /* t4(); */
    /* t5(); */
    /* t6(); */
    t7();
    return 0;
}
