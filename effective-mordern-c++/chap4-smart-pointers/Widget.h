#include <memory>
#include <string>
#include <vector>
#include "Gadget.hpp"
using namespace std;

class Widget {
public:
    Widget();
    // 析构函数的声明一定要加, 否则就是不完整类型
    ~Widget();

    // 不对,
    // 会导致未声明(因为编译器会在move构造函数内抛出异常的事件中生成析构pImpl的代码,
    // 而对pImpl 的析构要求Impl 具备完整类别)

    /* Widget(Widget&& rhs) = default; */
    /* Widget& operator=(Widget&& rhs) = default; */

    // error: invalid application of 'sizeof' to an incomplete type
    // 'Widget::Impl'
    Widget(Widget&& rhs);
    Widget& operator=(Widget&& rhs);

    // 复制
    Widget(const Widget& rhs);
    Widget& operator=(const Widget& rhs);

private:
    struct Impl; // 实现结构体
    unique_ptr<Impl> pImpl;
};

class Widget1 {
public:
    Widget1();
    // 此时不再有析构函数或移动操作的声明了

private:
    struct Impl; // 实现结构体
    shared_ptr<Impl> pImpl;
};
