#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;
// 转发的含义: 一个函数把自己的形参传递(转发)给另一个函数
// 目的: 让第二个函数(转发目的函数) 接受第一个函数(转发发起函数)所接受的同一对象
// 排除了按值传递形参(副本), 使转发目的函数能够处理原始传入对象,
// 并且不强迫调用者使用指针形参

// 涉及一般意义上的转发, 都是在处理形参是引用型别的情形

// 完美转发的含义是: 不仅转发对象, 还转发其显著特征`型别, 即: 左值/右值,
// 是否带有const / volatile饰词`等. 所以要结合万能引用来做

void f(const vector<int> x) {}

template <typename T>
void fwd(T&& param) {          // 接受任意实参
    f(std::forward<T>(param)); // 转发该实参到f
}
// 可变长形式
template <typename... Ts>
void fwd(Ts&&... params) {          // 接受任意实参
    f(std::forward<Ts>(params)...); // 转发所有实参到f
}
// 出现在: emplace系列函数以及智能指针的make系列函数

void t1() {
    // 转发失败的情况: 下面两语句执行的操作不同
    /* f(expression); */
    /* fwd(expression); */
    // case 1: 大括号初始化物
    f({1, 2, 3}); //{1,2,3}隐式转换为vector<int>
    /* fwd({1, 2, 3}); // error */
    // 非推导语境

    // 编译器先领受了调用端的实参型别, 又领受了f所声明的形参型别,
    // 编译器比较两个型别确定是否兼容, 如有必要, 会实施隐式类型转换使得调用成功
    // 直接调用f, 会完成上述转换, 但是对于fwd,
    // 并不会比较传入的实参和f声明的形参, 而是推导传递给fwd实参的型别结果,
    // 然后比较推导型别结果和f声明的形参型别 完美转发会在下面任一条件成立时失败:
    // 1. 编译器无法为一个或多个fwd形参推导出型别结果(编译失败)
    // 2. 编译器为一个或多个fwd形参推导出了错误的型别结果
    auto i1 = {1, 2, 3}; // initializer_list
    fwd(i1);             // ok
}


void t2() {
    // 以 0 或者NULL 代替空指针
}

class Widget {
public:
    // 声明
    static const size_t MinVals = 28;
};
// const size_t Widget::MinVals;
// 类外并未定义

void g(size_t val) { cout << val << endl; }
template <typename T>
void fwd1(T&& param) {         // 接受任意实参
    g(std::forward<T>(param)); // 转发该实参到g
}

void t3() {
    vector<int> widgetData;
    widgetData.reserve(Widget::MinVals);
    g(Widget::MinVals);
    // fwd1(Widget::MinVals); // link error: Widget::MinVals
    // 转发失败, 其实是与取地址(指针), 引用类似
    // 均发生链接错误
    // auto p = &Widget::MinVals;
    // const size_t& p = Widget::MinVals;
    // ld: symbol(s) not found for architecture arm64
}

// 等价的函数指针声明
void f1(int (*pf)(int)) {}
void f2(int pf(int)) {}

int processVal(int val) { return 1; }
int processVal(int val, int priority) { return 2; }
template <typename T>
void fwd2(T&& param) {          // 接受任意实参
    f1(std::forward<T>(param)); // 转发该实参到f
}

void t4() {
    // 重载的函数名和模板名, 也会导致完美转发失败
    cout << typeid(f1).name() << endl; // FvPFiiEE
    cout << typeid(f2).name() << endl; // FvPFiiEE

    f1(processVal); // ok, 但是fwd不行
    /* fwd2(processVal); */
    using ProcessFuncType = int (*)(int);
    ProcessFuncType processValPtr = processVal;
    fwd2(processValPtr); // ok
}
// 函数模板亦然
template <typename T>
T workOnVal(T param) {
    //
    return T();
}

void t41() {
    // 不知道使用函数模板的哪个实例
    /* fwd2(workOnVal); */
    using ProcessFuncType = int (*)(int);
    ProcessFuncType processValPtr = processVal;
    fwd2(static_cast<ProcessFuncType>(workOnVal));
}

// bit-field: 位域作函数实参

struct IPv4Header {
    uint32_t version : 4, IHL : 4, DSCP : 6, ECN : 2, totalLength : 16;
};
void h(size_t sz) { cout << sz << endl; }

template <typename T>
void fwd3(T&& param) {         // 接受任意实参
    h(std::forward<T>(param)); // 转发该实参到f
}
void t5() {
    IPv4Header h1;
    h(h1.totalLength);
    /* fwd3(h1.totalLength); // 非常量引用不能绑定到位域 */
    // 按值传递位域可以解决这个问题
    fwd3(static_cast<uint16_t>(h1.totalLength));
}

int main(int argc, char* argv[]) {
    /* t1(); */
    /* t2(); */
    t3(); 
    // t4();
    return 0;
}
