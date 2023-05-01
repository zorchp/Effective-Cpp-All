#include <iostream>
#include <memory>
#include <vector>

using namespace std;

namespace case0 {
class Base {
public:
    virtual void doWork() { cout << "do Base\n"; }
};

class Derived : public Base {
public:
    virtual void doWork() { cout << "do Derived\n"; }
};

} // namespace case0
void t1() {
    //
    using namespace case0;
    unique_ptr<Base> upb = make_unique<Derived>();
    upb->doWork(); // do Derived
}
// 使改写基类的函数生效要满足的条件:
//  1. 基类中的函数必须是虚函数
//  2. 基类和派生类中函数的名字必须完全相同(虚析构函数除外)
//  3. 基类和派生类中的函数形参型别必须完全一致
//  4. 基类和派生类中的函数常量性(constness)必须完全相同
//  5. 基类和派生类中的函数返回值和异常规格必须完全相同
//  **6. 基类和派生类中的函数引用修饰词必须完全相同(C++11)**

class Widget {
public:
    void doWork() & { cout << "lval\n"; }
    void doWork() && { cout << "rval\n"; }
};

Widget makeWidget() {
    // 工厂函数 return rval
    return Widget();
}
// 如果基类的虚函数带有引用饰词,
// 则派生类要对该函数进行改写版本也必须带有完全相同的引用饰词, 否则,
// 这些声明了的函数在派生类中依然存在, 只是不会改写基类中的任何函数

void t2() {
    Widget w;
    w.doWork();
    makeWidget().doWork();
    /* lval */
    /* rval */
}

// 所以下面的override声明就显得很有必要

namespace case1 {
class Base {
public:
    virtual void mf1() const;
    virtual void mf2(int x);
    virtual void mf3() &;
    virtual void mf4() const;
};

class Derived : public Base {
public:
    // 必须保证函数签名/常量性等都一致才行
    /* virtual void mf1() override; */
    virtual void mf1() const override;
    virtual void mf2(int x) override;
    virtual void mf3() & override;
    void mf4() const override;
};
} // namespace case1

// 重载: 引用型别也可以导致重载
void doSomething(Widget& w) {}
void doSomething(Widget&& w) {}

class Widget1 {
public:
    using DataType = vector<double>;
    DataType&
    data() & { // 引用饰词要加上, 否则会导致二义性(工厂函数返回临时对象,
               // 仍然可以调用左值版本)
        cout << "lval\n";
        return values;
    }
    DataType data() && {
        cout << "rval\n";
        return std::move(values);
    }

private:
    DataType values;
};

Widget1 makeWidget1() {
    // 工厂函数 return rval
    return Widget1();
}

void t3() {
    //
    Widget1 w;
    // 若没有给出右值版本的成员函数, 那么就都调用左值版本
    auto vals1 = w.data();             // 采用复制构造(左值)
    auto vals2 = makeWidget1().data(); // 右值
}

int main(int argc, char* argv[]) {
    /* t1(); */
    /* t2(); */
    t3();
    return 0;
}
