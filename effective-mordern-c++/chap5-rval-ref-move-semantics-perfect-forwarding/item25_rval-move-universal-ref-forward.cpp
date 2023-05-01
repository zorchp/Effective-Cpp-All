#include <iostream>
#include <memory>

using namespace std;
// 针对右值引用的最后一次使用实施move, 针对万能引用的最后一次使用实施forward


// 右值引用仅会绑定到那些可供移动的对象上, 如果形参型别为右值引用,
// 一定要确保其绑定的对象可移动(move)
class P {};

// 右值引用的使用:
class Widget {
public:
    Widget() {}
    // 希望把这些对象传递给其他函数时,
    // 使用一种允许该函数利用该对象右值性的方式(方法: move转为右值)
    Widget(Widget&& rhs) : name(std::move(rhs.name)), p(std::move(rhs.p)) {}

    // 只有在使用右值初始化来初始化时才会强制转换成右值型别(万能引用, forward)
    template <typename T>
    void setName(T&& newName) {
        name = std::forward<T>(newName);
    }

    template <typename T>
    void setName1(const string& newName) {
        name = std::move(newName); // 从常量左值取得赋值
    }
    template <typename T>
    void setName1(T&& newName) {   // 从右值取得赋值
        name = std::move(newName); // 可以编译, 但是行为不正确
    }

private:
    string name;
    shared_ptr<P> p;
};

string getWidgetName() { return string("123"); }

// 万能引用: forward , 右值引用: move, 如果混淆, 会带来一些问题

void t1() {
    //

    Widget w;
    auto n = getWidgetName();
    cout << n << endl; // 123
    w.setName1(n);     // move版本
    cout << n << endl; // 空值
}

void t2() {
    Widget w;
    const auto n = getWidgetName();
    cout << n << endl; // 123
    w.setName1(n);     // move版本
    cout << n << endl; // 123
}

void t3() { // 这样调用的话:
    Widget w;
    // 使用万能引用时候:
    //  字面值先传给setName, 然后传递给w内部的string的赋值运算符,
    //  这样才能使w的数据成员name直接从字面值得到赋值, 而不产生临时对象
    // 对于重载版本: 必须创建一个临时对象
    w.setName1("abc"); // move版本
}

class Matrix {};

// 按值返回, lhs是一个右值(所以其存储空间可以复用, 以保存矩阵之和)
Matrix operator+(Matrix&& lhs, const Matrix& rhs) {
    // 执行lhs += rhs;
    return std::move(lhs); // 这样就有了优化空间
}

class Fraction {};

template <typename T>
Fraction reduceCopy(T&& frac) {
    // 执行约分
    return std::forward<T>(frac); // 针对左值, 复制入返回值,
                                  // 对右值, 移动入返回值
}

// 针对被复制到返回值上的右值引用形参实施std::move
// 会有机会把复制构造转换成移动构造
// 但是相同的优化不能用于欲返回的局部变量上

Widget makeWidget() {
    Widget w;
    // 操作 w
    /* return w;            // 复制入返回值, 存在RVO(返回值优化) */
    return std::move(w); // 不对, 由于RVO条件不满足, 这是一个针对局部对象的引用
    // RVO 发生的条件:
    // 1. 局部对象型别和函数返回值的型别相同,
    // 2. 返回的就是局部对象自身
}

void t4() {
    //
    auto w = makeWidget();
    /* warning: moving a local object in a return statement prevents copy
     * elision [-Wpessimizing-move] */
}

int main(int argc, char* argv[]) {
    /* t1(); */
    /* t2(); */
    /* t3(); */
    t4();
    return 0;
}
