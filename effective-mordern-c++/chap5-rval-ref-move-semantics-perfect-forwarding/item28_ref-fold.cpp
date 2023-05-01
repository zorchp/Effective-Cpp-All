#include <iostream>
using namespace std;

template <typename T>
void func(T&& param) {}

class Widget {};
// 工厂函数
Widget makeWidget() { return Widget(); }

void t1() {
    //
    Widget w;
    func(w);            // 左值, T: Widget&
    func(makeWidget()); // 右值, T: Widget
}

void t2() {
    // ref to ref
    /* int x{}; */
    /* auto& & rx = x; //'rx' declared as a reference to a reference */
}

// 引用折叠的规则:
// 如果任一引用为左值引用, 则结果为左值引用, 否则(都为右值引用), 结果为右值引用

// 一种可能的实现(forward)
template <typename T>
T&& forward11(typename remove_reference<T>::type& param) {
    return static_cast<T&&>(param);
}
// C++14
template <typename T>
T&& forward14(remove_reference_t<T>& param) {
    return static_cast<T&&>(param);
}

// 引用折叠出现的语境:
/*
1. 模板实例化(常见)
2. auto 变量生成的型别

3. 生成和使用typedef 和别名声明(using )
4. decltype 的运用中
*/

void t3() {
    Widget w;
    auto&& w1 = w; // 发生引用折叠
    /* Widget& && w1 = w; */
    auto&& w2 = makeWidget();
    /* Widget&& w2 = makeWidget(); */
}
// 万能引用实际上就是满足下面两个条件的语境中的右值引用:
// 1. 型别推导过程中会区别左值和右值: T 左值推导为T&, T型别的右值推导为T
// 2. 会发生引用折叠

template <typename T>
class Widget1 {
public:
    typedef T&& RvalueRefToT;
};

void t4() {
    // typedef / using
    Widget1<int&> w;
    /* typedef int& && RvalueRefToT; */
    // 应用引用折叠
    typedef int& RvalueRefToT;
}

int main(int argc, char* argv[]) {
    t1();
    return 0;
}
