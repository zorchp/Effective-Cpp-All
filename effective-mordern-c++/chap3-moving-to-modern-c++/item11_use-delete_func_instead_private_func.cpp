#include <ios>
#include <iostream>

using namespace std;

// C++98代码
template <class charT, class traits = char_traits<charT>>
class basic_ios : public ios_base {
public:
    //
private:
    // 这样虽然阻止用户调用, 但是仍然可能有友元或者成员函数可以调用,
    // 导致链接期找不到函数定义而失败
    basic_ios(const basic_ios&);            // no defined
    basic_ios& operator=(const basic_ios&); // no defined
};

// C++11的代码:
template <class charT, class traits = char_traits<charT>>
class basic_ios : public ios_base {
public:
    //
private:
    // 这样虽然阻止用户调用, 但是仍然可能有友元或者成员函数可以调用,
    // 导致链接期找不到函数定义而失败
    basic_ios(const basic_ios&) = delete;
    basic_ios& operator=(const basic_ios&) = delete;
};

// 删除函数的好处在于, 任何函数都能成为`删除的`
bool isLucky(int number) { return number > 0; }
bool isLucky(char) = delete;
bool isLucky(bool) = delete;
bool isLucky(double) = delete; // double 和 float 都会被拒绝
// 当 float 类型面临类型转换到 double 还是 int 时候, 会首先选择 double

void t1() {
    // 下面的调用都是不合理的
    /* if (isLucky('a')) cout << "char\n"; */
    /* if (isLucky(true)) cout << "bool\n"; */
    /* if (isLucky(3.5)) cout << "double\n"; */
}

// 删除函数还可以阻止模板具现化(private做不到)
template <typename T>
void processPointer(T* ptr) {
    // do something
}

// 遇到字符指针时候和void*时候阻止具现化
template <>
void processPointer<void>(void*) = delete;
template <>
void processPointer<char>(char*) = delete;
template <>
void processPointer<const void>(const void*) = delete;
template <>
void processPointer<const char>(const char*) = delete;

int main(int argc, char* argv[]) {
    t1();
    return 0;
}
