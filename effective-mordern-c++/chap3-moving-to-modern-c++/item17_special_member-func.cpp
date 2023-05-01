#include <map>
#include <iostream>

using namespace std;

// 移动操作(移动构造, 移动赋值运算符)并不是相互独立的, 一旦声明了一个,
// 编译器就会阻止生成另一个, 因为生成一个自定义版本之后,
// 就表明编译器生成的默认版本会有区别, 所以另一个也不会被默认生成出来

// 移动操作的生成条件:
/*
1. 该类未声明任何复制操作
2. 该类未声明任何移动操作
3. 该类未声明任何析构函数
*/

class Widget {
public:
    ~Widget();
    // 仍然使用编译器的版本, 其实也说明编译器默认版本的行为是正确的
    Widget(const Widget&) = default;
    Widget& operator=(const Widget&) = default;
};

// 多态基类(拥有虚析构函数的基类), 使之拥有默认移动操作的方法:
class Base {
public:
    virtual ~Base() = default;
    Base(Base&&) = default; // 加上=default以生成移动操作
    Base& operator=(Base&&) = default;

    Base(const Base&) = default;
    Base& operator=(const Base&) = default;
};

class StringTable {
public:
    StringTable() {}
    ~StringTable() {
    } // 一段时间后加入了自定义的析构函数, 这个操作会阻止默认移动操作(降低性能)
    StringTable(StringTable&&) = default;            // 显式指定就好了
    StringTable& operator=(StringTable&&) = default; // 显式指定就好了
private:
    map<int, string> values;
};

// C++11 中, 这五类构造函数的生成规则:
/*
1. 默认构造函数: 仅当类中不含有用户声明的构造函数时才会自动生成
2. 析构函数: 析构函数默认是noexcept, 仅当基类虚析构, 派生类的析构函数才是虚的
3. 复制构造函数: 按成员进行非静态数据成员的复制构造,
仅当类中不包含用户声明的复制构造函数时才生成. 如果该类声明了移动操作,
则复制构造函数被删除, 在已经存在复制赋值运算符或析构函数的条件下,
仍然生成复制构造函数被废弃(deprecated)
4. 复制赋值运算符: 与复制构造函数类似
5. 移动构造函数和移动赋值运算符: 都按成员进行非静态数据成员的移动操作,
仅当类中不包含用户声明的复制操作, 移动操作和析构函数时才生成.
*/

// 对于成员函数模板的情况
// 成员函数模板并不会影响编译器生成默认的移动操作和复制操作
class Widget1 {
private:
    template <typename T>
    Widget1(const T& rhs);

    template <typename T>
    Widget1& operator=(const T& rhs);
};

void t1() {
    //
}
