#include <iostream>
#include <deque>
#include <utility>
using namespace std;
class Widget {};

const int i = 0;
bool f(const Widget& w) { return false; }
struct Point {
    int x, y;
};

void t1() {
    decltype(i) j = 10; // int
    decltype(f) func;   // bool(const Widget&);
    decltype(Point::x) x = 1;
    Widget w;
    /* if (func(w)) */
    /*     ; */
}

// not a very good way
template <typename Container, typename Index>
/* auto authAndAccess(Container& c, Index i) -> decltype(c[i]) { */
/* auto authAndAccess(Container& c, Index i) { */
decltype(auto) authAndAccess(Container& c, Index i) { // 非常量引用
    return c[i];
}

void t2() {
    //
    deque<int> d{1, 2, 3, 4, 5, 6};
    authAndAccess(d, 5) = 10; // 不可赋值(由于auto忽略引用性)
    auto w1 = Widget();
    decltype(auto) w2 = w1; // 保持引用性
}

// 工厂函数, 产生临时对象
deque<int> makeDeque() { return deque<int>({1, 2, 3}); }

template <typename Container, typename Index>
decltype(auto) authAndAccess(Container&& c,
                             Index i) { // 非常量右值引用(万能引用)
    /* return c[i]; */
    // 万能引用最好使用forward
    return std::forward<Container>(c)[i];
}
void t3() {
    auto s = authAndAccess(makeDeque(), 0); // ok
    cout << s << endl;                      // 1
}

void t4() {
    // 一个小坑, 关于括号:
    int x = 1;
    decltype(x) y = 2; // int
    // Non-const lvalue reference to type 'int' cannot bind to a temporary of
    // type 'int'
    decltype((x)) z = x; // non-const reference, int&
}

decltype(auto) f1() {
    int x = 0;
    return x;
}

decltype(auto) f2() {
    int x = 0;
    // Reference to stack memory associated with local variable 'x' returned
    return (x); // int&
}

void t5() {
    int x = f1();
    int y = f2();
    cout << x << endl; // 0
    cout << y << endl; // 0
}

int main(int argc, char* argv[]) {
    /* t1(); */
    /* t2(); */
    /* t3(); */
    /* t4(); */
    t5();
    return 0;
}
