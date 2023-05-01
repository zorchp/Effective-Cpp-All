#include <array>
#include <iostream>

using namespace std;

// constexpr 表示的值: 不仅是 const 的, 而且在编译阶段就已知
// 但是应用在函数中时, constexpr 函数返回的值既不是const的, 也并不是编译阶段已知


void t1() {
    int sz;
    /* constexpr auto arraySize1 = sz; // error, sz 值在编译期未知 */
    /* array<int, sz> data1;           // 同上 */
    constexpr auto arraySize2 = 10; // 10是编译期常量
    array<int, arraySize2> data2;
}

void t2() {
    int sz;
    const auto arraySize = sz; // ok, const 对象不一定经由编译期已知值来初始化
}
// 所有的constexpr对象都是const的, 但是并非所有的const对象都是constexpr对象

// 如果想让编译器提供一个保证, 让变量拥有一个值, 用于要求编译期常量的语境,
// 那么能达到这个目的的工具是constexpr.


// 对于constexpr函数来说:
//  如果传入的是编译期常量, 则产出编译期常量; 如果传入的是直至运行期才知道的值,
//  则产出运行期值

/*
constexpr函数可以用在要求编译期常量的语境中, 此时若传入的实参是编译期已知的,
则结果也会在编译期间计算出来, 如果有任何一个实参值在编译期未知, 则不会通过编译

constexpr 函数调用中, 如果传入的值有一个或多个在编译期未知,
则运作方式与普通函数无异(运行期才计算结果)

也就是说, 一个constexpr函数可以同时满足要求
*/

constexpr int pow(int base, int exp) noexcept {
    // c++11对于constexpr只能支持一条语句
    /* return (exp == 0 ? 1 : base * pow(base, exp - 1)); */
    int res{1};
    for (int i{}; i < exp; ++i) res *= base;
    return res;
}

constexpr auto numConds = 5;
array<int, pow(3, numConds)> res;


class Point {
public:
    constexpr Point(double xVal = 0, double yVal = 0) noexcept
        : x(xVal), y(yVal) {}
    constexpr double xValue() const noexcept { return x; }
    constexpr double yValue() const noexcept { return y; }
    // C++14中,
    // 可以为可以更改数据成员的成员函数(并且是返回值为void的成员函数)指定constexpr
    constexpr void setX(double newX) noexcept { x = newX; }
    constexpr void setY(double newY) noexcept { y = newY; }

private:
    double x, y;
};

constexpr Point midpoint(const Point &p1, const Point &p2) noexcept {
    return {(p1.xValue() + p2.xValue()) / 2, (p1.yValue() + p2.yValue()) / 2};
}

constexpr Point reflection(const Point &p) noexcept {
    Point res;
    res.setX(-p.xValue());
    res.setY(-p.yValue());
    return res;
}

void t3() {
    constexpr Point p1(1.2, 2.3);
    constexpr Point p2(1.2, 2.3);
    constexpr auto mid = midpoint(p1, p2);
    constexpr auto reflectedMid = reflection(mid);
}


int main(int argc, char *argv[]) {
    t3();
    return 0;
}
