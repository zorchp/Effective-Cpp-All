#include <iostream>
#include <memory>
#include "Widget.h"
using namespace std;

// Pimpl : Pointer to Implementation
// 指涉到实现的指针:
// 就是把某一个类的数据成员用一个指涉到某实现类(或者结构体)的指针替代,
// 而后把在主类中的数据成员放置到实现类中
// 并且通过这些指针访问数据成员

namespace case0 {

class Gadget {};

class Widget {
public:
    Widget();
    ~Widget(); // 析构函数是必要的

private:
    // 采用数据成员, 在自定义类型改动之后, 导致重复编译
    /* string name; */
    /* vector<double> data; */
    /* Gadget g1, g2, g3; */
    struct Impl; // 实现结构体
    Impl* pImpl; // 指涉到它的指针
};

struct Widget::Impl {
    string name;
    vector<double> data;
    Gadget g1, g2, g3;
};

Widget::Widget() : pImpl(new Impl) {}
Widget::~Widget() { delete pImpl; }
} // namespace case0


// 还不够好, 使用智能指针试试:

namespace case1 {

class Gadget {};

class Widget {
public:
    Widget();

private:
    struct Impl; // 实现结构体
    unique_ptr<Impl> pImpl;
};

struct Widget::Impl {
    string name;
    vector<double> data;
    Gadget g1, g2, g3;
};

Widget::Widget() : pImpl(std::make_unique<Impl>()) {}
// 此时不需要析构函数了(由智能指针接管内存释放)

} // namespace case1

void t1() {
    //
    /* using namespace case1; */
    Widget w;
}

void t2() {
    Widget1 w;
    auto w2(std::move(w)); // move 构造
    w = std::move(w2);     // move赋值
}

int main(int argc, char* argv[]) {
    t1();
    return 0;
}
