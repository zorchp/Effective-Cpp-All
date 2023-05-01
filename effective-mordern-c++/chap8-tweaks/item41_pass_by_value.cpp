#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 对于可复制的形参, 在移动成本低并且一定会被复制的前提下, 考虑将其按值传递
/*
采用赋值方式复制形参的函数, 其按值传递带来的额外成本取决于:
传入的型别,
左值和右值实参的占比, 型别是否使用动态分配内存, 还有,
在确实使用了动态分配内存的前提下, 该型别的赋值运算符如何实现,
以及与赋值目标相关联的内存是否至少与赋值源相关联的内存尺寸相当的可能性高低

*/

class Widget {
public:
    // 冗余, 写了两份实现(需要两份文档)
    void addName(const string& newName) {
        names.push_back(newName); // 接受左值
    }
    void addName(const string&& newName) {
        names.push_back(std::move(newName)); // 接受右值
    }

private:
    vector<string> names;
};

class Widget1 {
public:
    // 针对成员模板的实现(但是需要生成很多份代码)
    // 左值/右值实例化代码不同, 针对string和可以转型为string的代码实例化也不同
    template <typename T>
    void addName(T&& newName) {
        names.push_back(std::forward<T>(newName));
    }

private:
    vector<string> names;
};

class Widget2 {
public:
    // 新方法, 既接受左值, 也接受右值(按值传递, 传入左值时需要拷贝一份)
    void addName(string newName) { names.push_back(std::move(newName)); }

private:
    vector<string> names;
};
