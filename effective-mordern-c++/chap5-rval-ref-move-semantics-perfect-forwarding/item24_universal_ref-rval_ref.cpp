#include <iostream>
#include <memory>
#include <utility>
#include <vector>

using namespace std;

class Widget {};


// 即使涉及型别推导, 由于不是T&&形式, 所以也不是万能引用
template <typename T>
void f(vector<T>&& param) {} // rval-ref


// T&& 代表右值引用, 也可以代表万能引用(因为也可以像左值引用一样运作)

// 万能引用: 两种场景
// 1. 函数模板的形参

template <typename T>
void g(T&& param) {
    cout << "universal ref\n";
} // non-rval-ref, 万能引用

// 2. auto声明
Widget&& var1 = Widget(); // rval-ref
auto&& var2 = var1;       // non-rval-ref, var2 代表万能引用

// 上面的两种使用场景的共同之处在于, 都涉及型别推导

// 下面的例子中, 不涉及型别推导, 则是右值引用
void f(Widget&& param);
Widget&& w2 = Widget();

void t1() {
    //
    Widget w;
    g(w);
    g(std::move(w));

    vector<int> v;
    /* f(v); */
    f(std::move(v));
}

// 加了const也不是万能引用
template <typename T>
void h(const T&& param) {}

template <class T, class Allocator = allocator<T>>
class Vector {
public:
    // 虽然形式正确(T&&), 但是由于不涉及型别推导, 仍不是万能引用
    void push_back(T&& x);
    // 因为可以通过指定类型进行 具现化

    // emplace_back 存在万能引用:
    template <class... Args>
    void emplace_back(Args... args);
    // 型别形参Args 独立于vector 的型别形参T, 所以每次emplace_back 调用,
    // Args都要进行推导(一个形参包, 而非单一形参型别)
};

// 万能引用(形式一致即可, 不一定非要用T)
template <typename MyType>
void someFunc(MyType&& param);

// auto 可以做万能引用(但是不如模板形参推导那样常见)
auto timeFunc = [](auto&& func, auto&&... params) {
    std::forward<decltype(func)>(func)(             // 调用func
        std::forward<decltype(params)>(params)...); // 取用params
};

int main(int argc, char* argv[]) {
    t1();
    return 0;
}
