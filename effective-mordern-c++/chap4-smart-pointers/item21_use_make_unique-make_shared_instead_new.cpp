#include <cstdint>
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>
using namespace std;

// 一个可能的实现(C++11), 不支持数组和自定义析构器
template <typename T, typename... Ts>
unique_ptr<T> make_unique1(Ts &&...params) {
    return unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

class Widget {};

void t1() {
    // 使用或者不使用make系列函数
    shared_ptr<Widget> spw(new Widget);
    auto spw1 = make_shared<Widget>();
    unique_ptr<Widget> upw(new Widget);
    auto upw1 = make_unique<Widget>();
}

// 不仅是形式上的简洁, make_shared 也会保证异常安全, 因为new是表达式评估,
// 而make_shared是针对shared_ptr构造函数的调用

// 另外, 先new创建对象然后让shared_ptr指向对象会导致控制块的内存占用变大,
// 因为new 是先为Widget分配一次内存, 然后对控制块分配一次内存,
// make_shared只分配一次内存, 就减小了程序的静态尺寸(make_shared
// 也会避免控制块中的一些簿记信息的必要性)

// 但是, make_shared也有一些局限性, 尤其是不能自定义析构器, 另外,
// 针对initializer_list的区分也有不同:
void t2() {
    auto upv = make_unique<vector<int>>(10, 20);
    auto spv = make_shared<vector<int>>(10, 20);
    // 创建的是10个元素, 而非元素10, 20, 说明在make系列函数中,
    // 对形参进行完美转发的代码使用()而非{}
    cout << upv->size() << endl; // 10
    cout << spv->size() << endl; // 10

    auto initList = {10, 20};
    auto spv1 = make_shared<vector<int>>(initList);
    auto upv1 = make_unique<vector<int>>(initList);
    cout << spv1->size() << endl; // 2
    cout << upv1->size() << endl; // 2
}

// 上述情形对shared_ptr和unique_ptr来说等价,
// 但是还有两种特殊情况仅存在于shared_ptr上:

// 1. 针对类内重载的operator new来说,
// shared_ptr支持的自定义分配器(通过allocate_shared完成)以及自定义析构器,
// 因为没法加上控制块

// 2. 控制块中还有一部分信息成为弱计数,
// 用来针对指涉到该控制块的weak_ptr进行计数(但是并不一直等于指涉到控制块的weak_ptr的数量),
// 通过检查控制块内的引用计数来检查自己是否失效
// 这时候, 假如引用计数为0, 则 weak_ptr失效,
// 但是weak_ptr还指向了某一个控制块(弱计数 >0 ),
// 这样就导致通过对应于shared_ptr的 make 系列函数所分配的内存在最后一个
// shared_ptr 和最后一个指涉到它的weak_ptr 都被析构之前, 无法得到释放

// 在控制块比较大, 并且动态内存也比较大的时候, 如果最后一个shared_ptr
// 被析构和最后一个weak_ptr 析构之间的时间间隔不能忽略时,
// 在对象析构和内存释放之间就产生了延迟

// 例如:
class ReallyBigType {};

auto pBigObj = make_shared<ReallyBigType>();
// 如果通过make_shared 来创建资源占用较大的对象, 并且应用weak_ptr
// 使用这两种智能指针管理这些资源(多个) 最后一个指涉到该对象的 shared_ptr

// 在此被析构, 但指涉到该对象的若干 weak_ptr 仍然存在,
// 在此阶段前述大内存对象所占用的内存仍然未被回收

// ... 在对象析构和内存释放之间产生延迟...

// 最后一个指涉到该对象的 weak_ptr 在此被析构,
// 控制块和对象所占用的同一内存块在此被释放


// new 则不会

// 异常安全的处理函数:

void processWidget(shared_ptr<Widget> spw, int priority) {}

void cusDel(Widget *ptr) {
    cout << "deleted...\n";
    delete ptr;
}
int computePriority() {
    //
    return 1;
}

void t3() {
    // 存在资源泄露
    processWidget(shared_ptr<Widget>(new Widget, cusDel), computePriority());
    // 若computePriority()调用在`new Widget`之后, 但在shared_ptr 的构造函数之前,
    // 若computePriority发生了异常, 那么动态分配的 Widget 将会泄露
}

// 异常安全的调用版本:
void t4() {
    shared_ptr<Widget> spw(new Widget, cusDel);
    processWidget(spw, computePriority());
}
// 存在拷贝操作, 可以通过move转为右值来减少拷贝次数(提高性能)
void t5() {
    // 既有效率, 又是异常安全的
    shared_ptr<Widget> spw(new Widget, cusDel);
    processWidget(std::move(spw), computePriority());
}

int main(int argc, char *argv[]) {
    /* t2(); */
    /* t3(); */
    /* t4(); */
    t5();
    return 0;
}
