#include <iostream>
#include <memory>
#include <utility>
#include <vector>

using namespace std;

// shared_ptr
// 采用共享所有权来管理其生存期(没有哪个特定的shared_ptr确切拥有某一对象)
// 可以通过访问某资源的引用计数来确定是否自己是最后一个指涉到该资源的
// 通过引用计数的方式(所以也导致内存占用变大)

// 规则:
// 1. 析构函数递减引用计数
// 2. 拷贝赋值运算符执行两种操作(针对两个指向不同对象的shared_ptr来说),
//  例如对sp1 = sp2, 将修改sp1, 使其指向 sp2 指向的对象, 其结果为:
//  sp1 指向的对象的引用计数递减, sp2 指向的对象的引用计数递增
// 3. 引用计数为零, 所指对象自动销毁

// 对性能的影响
/*
1. shared_ptr大小是裸指针的两倍
2. 引用计数的内存必须动态分配
3. 引用计数增减必须是原子操作(atomic), 而一般来说原子操作要比非原子操作速度慢
*/
void t1() {
    //
    shared_ptr<int> sp = make_shared<int>(1);
    cout << sizeof(sp) << endl;                  // 16
    cout << sizeof(make_shared<int>(1)) << endl; // 16
}

class Widget {};

auto loggingDel = [](Widget *pw) {
    // make log Entry
    cout << "deleting...\n";
    delete pw;
};

void t11() {

    // 使用unique_ptr的时候, 析构器作为智能指针类型的一部分
    unique_ptr<Widget, decltype(loggingDel)> upw(new Widget, loggingDel);

    // 使用shared_ptr时, 析构器不是智能指针类型的一部分
    shared_ptr<Widget> spw(new Widget, loggingDel);
    // 由此可见, shared_ptr的设计灵活性更好
    /* deleting... */
    /* deleting... */
}

// 同一类型的不同对象(只因为删除器不同),
// 可以放在一个容器中(只是对于shared_ptr来说)

void t2() {
    //
    auto customDel1 = [](Widget *pw) {};
    auto customDel2 = [](Widget *pw) {};

    shared_ptr<Widget> pw1(new Widget, customDel1);
    shared_ptr<Widget> pw2(new Widget, customDel2);
    vector<shared_ptr<Widget>> vpw{pw1, pw2};
    // 并且可以互相赋值
}
// 与unique_ptr不同的是,
// 自定义析构器(即使是通过函数指针)不会导致shared_ptr所占内存变大(一直都是裸指针的两倍大)
// 原因是shared_ptr仅保存了两个指针(一个指向目标对象, 一个指向控制块对象,
// 引用计数, 弱计数, 自定义析构器等部分都属于控制块中)
// 一个对象的控制块由创建首个指涉到该对象的shared_ptr的函数来确定,
// 正在创建指涉到某对象的shared_ptr的函数无法知道是否有其他的shared_ptr已经指涉到该对象,
// 控制块的创建遵循下面的规则:
/*
1. make_shared 总是创建一个控制块,
2. 从具备专属所有权的指针(例如unique_ptr)出发构造一个shared_ptr时,
 会创建一个控制块.
3. shared_ptr构造函数使用裸指针来作为实参调用时, 会创建一个控制块
*/

// 针对裸指针的测试: 用shared_ptr指向一个由裸指针指向的动态内存块
void t3() {
    auto pw = new Widget;
    shared_ptr<Widget> spw1(pw, loggingDel); // 此时为*pw创建一个控制块
    shared_ptr<Widget> spw2(pw, loggingDel); // 为*pw创建了一个新的控制块
    // 析构两次*pw, 所以*pw 有了两个引用计数, 最后函数作用域结束, 引用计数变为0,
    // 导致*pw被析构两次, 第二次析构导致了未定义行为
    /* deleting... */
    /* deleting... */
    /* ================================================================= */
    /* ==57885==ERROR: AddressSanitizer: attempting double-free on
     * 0x000103800690 in thread T0: */
}

// 一个比较好的改进:
void t4() {
    shared_ptr<Widget> spw1(new Widget, loggingDel); // 直接传递new 表达式
    shared_ptr<Widget> spw2(spw1); // 此时spw1和spw2使用的是同一个控制块
    /* deleting... */
}

// 测试
class Widget1 {
public:
    void process();
};

vector<shared_ptr<Widget1>> processWidgets;
void Widget1::process() {
    cout << "process\n";
    processWidgets.emplace_back(this); // 将裸指针this传入了shared_ptr容器
    // 由此构建的shared_ptr将为其所指涉到的Widget1类型的对象*this创建一个新的控制块
}

void t5() {
    //
    Widget1 w1;
    w1.process(); // TODO??
    /* process */
    /* ==69193==ERROR: AddressSanitizer: attempting free on address which was
     * not malloc()-ed: 0x00016fddac80 in thread T0 */
}

void t6() {
    auto pw1 = new Widget1;
    pw1->process(); // process
}

void t7() {
    shared_ptr<Widget1> sppw1 = make_shared<Widget1>();
    sppw1->process();
    /* process */
    /* ==71650==ERROR: AddressSanitizer: attempting free on address which was
     * not malloc()-ed: 0x000103202038 in thread T0 */
}

class Widget2;
vector<shared_ptr<Widget2>> processWidgets2;
// 针对智能指针内部存放this指针的情况, 有一个模板类可以用来继承
class Widget2 : public std::enable_shared_from_this<Widget2> {
public:
    template <typename... Ts>
    static shared_ptr<Widget2> create(Ts &&...params) {
        return shared_ptr<Widget2>(new Widget2(std::forward<Ts>(params)...));
    }
    void process();

private:
    // ctor
    Widget2() { cout << "call ctor\n"; }
};

void Widget2::process() {
    cout << "Widget2::process\n";
    processWidgets2.emplace_back(shared_from_this());
}


// 为了避免用户在shared_ptr指涉到该对象(被管理的对象)前就调用了引发shared_from_this的成员函数,
// 继承自enable_shared_from_this的类通常会将其构造函数声明为private访问层级,
// 并且只允许通过调用返回shared_ptr的工厂函数来创建对象

void t8() {
    // 这样处理自身就没问题了
    /* auto sppw2 = make_shared<Widget2>(); */
    auto sppw2 = Widget2::create();
    sppw2->process(); // Widget2::process
}


int main(int argc, char *argv[]) {
    /* t1(); */
    /* t11(); */
    /* t2(); */
    /* t3(); */
    /* t4(); */
    /* t5(); */
    /* t6(); */
    /* t7(); */
    t8();
    return 0;
}
