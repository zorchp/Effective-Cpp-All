#include <initializer_list>
#include <iostream>
#include <vector>
#include <typeinfo>

using namespace std;

void t1() {
    int x = 27;
    const int cx = x;
    const int& rx = x;
    // 与函数模板的实参型别推导类似
    auto x1 = 28;
    const auto cx1 = x1;
    const auto& rx1 = x1;
}

void someFunc(int, double) {}
void t2() {
    const char name[] = "hello";
    const char* PtrName = name;
    // 与函数模板的实参型别推导类似
    auto arr1 = name;
    auto& arr2 = name;

    // 与函数模板的实参型别推导类似
    auto func1 = someFunc;
    auto& func2 = someFunc;
}

void t3() {
    // 都是数字
    int x1 = 27;
    int x2(27);
    int x3{27};
    int x4 = {27};
    // 后两个是initializer_list<int>
    auto y1 = 27;
    auto y2(27);
    auto y3 = {27};
    auto y4{27};
    cout << typeid(y3).name() << endl; // St16initializer_listIiE
    cout << typeid(y4).name() << endl; // i
}

template <typename T>
void f(T param) {
    cout << typeid(T).name() << endl; //
}
template <typename T>
void g(initializer_list<T> param) {
    cout << typeid(T).name() << endl; //
}
template <template <typename> class Container, typename T>
void h(Container<T> param) {
    cout << typeid(T).name() << endl;            //
    cout << typeid(Container<T>).name() << endl; //
}


void t4() {
    auto x = {1, 2, 3};

    /* f({1, 2, 3}); // error: no matching function for call to 'f' */
    g({1, 2, 3}); // ok
    /* h(vector<int>{1, 2, 3}); // ok, only for g++ */
}
// error
auto createInit_list() {
    /* return {1, 2, 3}; */
} // Cannot deduce return type from initializer list

void t5() {
    vector<int> v;
    auto resetV = [&](const auto& newVal) { v = newVal; };
    // error
    /* resetV({1, 2, 3}); */
}
int main(int argc, char* argv[]) {
    /* t1(); */
    /* t2(); */
    /* t3(); */
    t4();
    return 0;
}
