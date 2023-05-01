#include <memory>
#include <regex>
#include <string>
#include <list>
#include <vector>
#include <iostream>

using namespace std;

/* C++11 一个可能的实现:(vector)
template <class T, class Allocator = allocator<T>>
class vector {
public:
    void push_back(const T& x);
    void push_back(T&& x);
};
*/

void t1() {
    //
    vector<string> vs;
    vs.push_back("xwyyz");
    // 先创建临时对象string("xwyyz"), 为一个右值
    // 传递给push_back的右值重载版本, 在那里被绑定到右值引用形参x,
    // 然后创建一个x的副本, 此构造即为移动构造, (第二次构造了)
    // push_back 返回的时候, 临时对象被析构, 调用 string 的析构函数
}


void t2() {
    vector<string> vs;
    // 使用emplace系列函数, emplace_back 使用了完美转发
    // 置入 而非 插入, 原因在于 置入的主要优点就是不会创建也不会析构临时对象
    vs.emplace_back("xwyyz");
}

// 如果下列条件都成立, 则置入将比插入更高效:
// 1. 欲添加的值是以构造而非赋值方式加入容器(赋值的话两者在性能上没有太大区别)
// 2. 传递的实参型别与容器持有之物的型别不同
// 3. 容器不太可能由于出现重复情况而拒绝待添加的新值

void t3() {
    vector<string> vs;
    vs.emplace_back("1");
    vs.emplace_back("1");
    vs.emplace_back("1");
    vs.emplace(vs.begin(), "xwyyz");
}

class Widget {};
void killWidget(Widget* ptr) {
    delete ptr; // 自定义删除器
}

void t4() {
    list<shared_ptr<Widget>> ptrs;
    // 两种写法等价
    ptrs.push_back(shared_ptr<Widget>(new Widget, killWidget));
    ptrs.push_back({new Widget, killWidget});
    // push_back 会保证异常安全, 但是 emplace_back 不一定
    ptrs.emplace_back(
        new Widget,
        killWidget); // 先完美转发裸指针, 然后运行到分配结点内容的执行点
    // 问题在于, 要让智能指针(资源管理类)发挥作用, 前提是资源(new 出来的裸指针)
    // 会立即传递给资源管理对象的构造函数,
    // (make_shared系列函数就会完成这个过程(自动化))
    // 但是 置入函数中, 完美转发会推迟资源管理对象的创建,
    // 直到他们能够在容器的内存中构造为止
    // 一个安全的实现:
    shared_ptr<Widget> spw(new Widget, killWidget);
    ptrs.push_back(std::move(spw));
    ptrs.emplace_back(std::move(spw));
}

void t5() {
    // 正则表达式容器
    vector<regex> regexes;
    // 调用直接初始化, 会尽量找到一个满足条件的构造函数
    regexes.emplace_back(nullptr); // ok??
    // 下面两种是直接初始化
    regex r(nullptr);  // 相当于传递的是构造函数实参
    regex r2{nullptr}; // 相当于传递的是构造函数实参
    // 复制初始化(采用等号), 不允许调用带有explicit 声明饰词的构造函数
    /* regex r1 = nullptr;            */
    // push_back 调用复制初始化, 不允许explicit 构造函数
    /* regexes.push_back(nullptr); // error */
}

int main(int argc, char* argv[]) {
    t4();
    return 0;
}
