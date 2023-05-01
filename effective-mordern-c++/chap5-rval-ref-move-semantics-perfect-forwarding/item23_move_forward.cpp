#include <cstddef>
#include <iostream>
#include <type_traits>
#include <utility>
#include <chrono>

using namespace std;
// std::move: 一个可能的实现
template <typename T>
typename remove_reference<T>::type&& move1(T&& param) {
    using ReturnType = typename remove_reference<T>::type&&;
    return static_cast<ReturnType>(param);
}

// 基于C++14 实现
template <typename T>
decltype(auto) move2(T&& param) {
    using ReturnType = remove_reference_t<T>&&;
    return static_cast<ReturnType>(param);
}

class Annotation {
public:
    // 虽然使用了move, 但是text还是被复制进value的,
    // 因为move仅仅强制类型转换为右值, 但是text是const的, 所以在转换之前,
    // 为一个左值const string 对象, 转换之后还是const string, 常量性保留
    explicit Annotation(const string text) : value(std::move(text)) {}

private:
    string value;
};

// move 不会检查实参类型, 而 forward 会检查
class Widget {};
void process(const Widget& lvalArg) { cout << "process lval\n"; }
void process(Widget&& rvalArg) { cout << "process rval\n"; }

template <typename T>
void logAndProcess(T&& param) {
    auto now = chrono::system_clock::now();
    /* make log entry */
    // 如果用move, 全调用右值版本, 也即, 始终执行强制类型转换
    /* process(std::move(param)); */
    process(std::forward<T>(param));
    // 仅当其实参是使用右值完成初始化时, 才会执行向右值型别的强制类型转换
}

void t1() {
    //
    Widget w;
    logAndProcess(w);
    logAndProcess(std::move(w));
}

class Widget1 {
public:
    // 使用 move: 1. 方便, 2. 减少错误可能(forward需要指定类型) 3. 更加清晰
    Widget1(Widget1&& rhs) : s(std::move(rhs.s)) { ++moveCtorCalls; }

private:
    static size_t moveCtorCalls;
    string s;
};
size_t Widget1::moveCtorCalls = 0;

// 如果采用forward, 就是下面这样:
class Widget2 {
public:
    // forward 需要传入的是一个非引用型别
    Widget2(Widget2&& rhs) : s(std::forward<string>(rhs.s)) { ++moveCtorCalls; }

private:
    static size_t moveCtorCalls;
    string s;
};
size_t Widget2::moveCtorCalls = 0;

// move: 无条件地向右值型别的强制类型转换, 为移动操作作铺垫,
// forward: 仅仅对绑定到右值的引用实施向右值型别的强制类型转换,
// 传递(转发)一个对象到另一个函数,
// 在此过程中无论是该对象原始型别具备左值性还是右值性, 都保持原样

void t2() {
    //
}

int main(int argc, char* argv[]) {
    t1();
    return 0;
}
