#include <iostream>
#include <iterator>
#include <vector>
#include <atomic>

using namespace std;

void t1() {
    // 对内建类型来说, 使用等号就是一次赋值
    int x(0);
    int y = 0;
    int z{0};
    int w = {0};
}
namespace case0 {
class Widget {
public:
    Widget() { cout << "call default ctor\n"; }
    Widget(int i) : m_i(i) { cout << "call Widget(int) ctor\n"; }
    Widget(const Widget& rhs) : m_i(rhs.m_i) { cout << "call copy ctor\n"; }

    Widget& operator=(const Widget& rhs) {
        cout << "call assignment operator\n";
        if (&rhs == this) return *this;

        this->m_i = rhs.m_i;
        return *this;
    }

    ~Widget() { cout << "call dtor\n"; }

private:
    int m_i;
    // 为非静态成员指定默认初始化值
    int x{0};
    int y = 0;
    // error
    /* int z(0); */
};
} // namespace case0

// 对用户自定义类型:
void t2() {
    case0::Widget w1;      // 默认构造函数
    case0::Widget w2 = w1; // 拷贝构造函数
    w1 = w2;               // 拷贝赋值运算符
    /* call default ctor */
    /* call copy ctor */
    /* call assignment operator */
    /* call dtor */
    /* call dtor */
}

void t3() {
    vector<int> v{1, 2, 3};
    atomic<int> ai1{0};
    atomic<int> ai2(0);
    /* atomic<int> ai3 = 0; // error */
}

void t4() {
    // braced initialization
    // 用来避免烦人的分析机制(函数指针)
    using namespace case0;
    Widget w;
    Widget w2(1);
    /* Widget w3();  */
    // Empty parentheses interpreted as a function declaration
    Widget w4{}; // ok
    /* call default ctor */
    /* call Widget(int) ctor */
    /* call default ctor */
    /* call dtor */
    /* call dtor */
    /* call dtor */
}

// 在构造函数被调用时, 只要形参中没有任意一个具备initializer_list型别,
// 那么小括号和大括号在使用上就没有区别
namespace case1 {
class Widget {
public:
    Widget(int i, bool b) { cout << "int, bool\n"; }
    Widget(int i, double d) { cout << "int, double\n"; }
};
} // namespace case1

void t5() {
    using namespace case1;
    case1::Widget w1(10, true);
    case1::Widget w2{10, true};
    case1::Widget w3(10, 5.0);
    case1::Widget w4{10, 5.0};
    /* int, bool */
    /* int, bool */
    /* int, double */
    /* int, double */
}

// 如果有一个或多个构造函数声明了任何一个具备initializer_list型别的形参,
// 那么采用了{}初始化的语句会优先选用带有initializer_list型别形参的重载版本

namespace case2 {
class Widget {
public:
    Widget(int i, bool b) { cout << "int, bool\n"; }
    Widget(const Widget& rhs) : data(rhs.data) { cout << "copy ctor\n"; }
    Widget(Widget&& rhs) : data(rhs.data) {
        /* rhs = nullptr; */
        cout << "move ctor\n";
    }
    Widget(int i, double d) { cout << "int, double\n"; }
    Widget(initializer_list<long double> il) { cout << "initializer_list\n"; }
    operator float() const {
        cout << "operator float\n";
        return static_cast<float>(data);
    }

private:
    int data;
};
} // namespace case2

void t6() {
    using namespace case2;
    Widget w1(10, true);
    Widget w2{10, true}; // 使用初始化列表, 并且包含了隐式类型转换
    Widget w3(10, 5.0);
    Widget w4{10, 5.0}; // 使用初始化列表, 并且包含了隐式类型转换
    /* int, bool */
    /* initializer_list */
    /* int, double */
    /* initializer_list */
    Widget w5(w4); // 拷贝构造
    // 即使是平常会执行复制或移动的构造函数也可能被带有initializer_list型别形参的构造函数劫持
    Widget w6{w4}; // initializer_list构造函数, w4返回值先被转换为float,
                   // 然后转换为long double(为配合initializer_list)
    Widget w7(std::move(w4)); // 搬移构造函数
    // 即使是平常会执行复制或移动的构造函数也可能被带有initializer_list型别形参的构造函数劫持
    Widget w8{std::move(w4)};

    cout << w5 << endl;
    cout << w6 << endl;
    // just for g++, clang++ 不成立
    /* copy ctor */
    /* operator float */
    /* initializer_list */
    /* move ctor */
    /* operator float */
    /* initializer_list */
    /* operator float */
    /* 0 */
    /* operator float */
    /* 1.5863e+09 */
}

void t7() {

    // vector使用()或者{}初始化的不同
    vector<int> v1(10, 20), v2{10, 20};
    for (auto i1 : v1) cout << i1 << " ";
    cout << endl;
    for (auto i2 : v2) cout << i2 << " ";
    cout << endl;

    /* 20 20 20 20 20 20 20 20 20 20  */
    /* 10 20  */
}

// 可变参数模板的情况
template <typename T, typename... Ts>
void doSomeWork(Ts&&... params) {
    // 创建局部对象T
    T localObject1(std::forward<Ts>(params)...); // 使用n个m的方式构造局部变量
    T localObject2{
        std::forward<Ts>(params)...}; // 使用initializer_list构造局部变量
}

void t8() {
    // 调用上述模板
    vector<int> v;
    doSomeWork<vector<int>>(10, 20);
}


int main(int argc, char* argv[]) {
    /* t1(); */
    /* t2(); */
    /* t4(); */
    /* t5(); */
    /* t6(); */
    t7();
    return 0;
}
