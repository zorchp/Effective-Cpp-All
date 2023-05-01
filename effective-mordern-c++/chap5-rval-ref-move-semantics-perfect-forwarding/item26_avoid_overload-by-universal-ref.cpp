#include <iostream>
#include <iterator>
#include <set>
#include <string>

using namespace std;

multiset<string> names;
namespace case0 {
void logAndAdd(const string& name) {
    // get-current-time
    // log(now, "logAndAdd");
    names.emplace(name);
}

} // namespace case0

void t1() {
    using namespace case0;
    // 调用
    string petName("Darla");
    logAndAdd(petName);       // 左值string
    logAndAdd(string("Laa")); // 右值string
    logAndAdd("Dog");         // 字符串字面量
}

// 效率较高的版本:

template <typename T>
void logAndAdd(T&& name) {
    // get-current-time
    // log(now, "logAndAdd");
    names.emplace(std::forward<T>(name));
}

// 考虑一个重载版本: 从idx读取string
string nameFromIdx(int idx) { return to_string(idx); }

void logAndAdd(int idx) { names.emplace(nameFromIdx(idx)); }


void t2() {
    // 调用
    string petName("Darla");
    logAndAdd(petName);       // 左值string
    logAndAdd(string("Laa")); // 右值string
    logAndAdd("Dog");         // 字符串字面量

    logAndAdd(1);
    short aa = 1;
    /* logAndAdd(aa); // error! */
    // logAndAdd 有两个重载版本, 形参型别为T&&的版本可以将T推导为short,
    // 从而产生一个精确匹配,
    // 而形参型别为int的版本只能在型别提升以后才能匹配到short型别的实参
    // 按照重载决议规则, 精确匹配优先于提升后才能匹配, 所以会因为找不到
    // short版本的string构造函数而报错
}
// 形参为万能引用的函数, 是C++中最贪婪的(总是去寻找精确匹配)


class Person {
public:
    template <typename T>
    explicit Person(T&& n) : name(std::forward<T>(n)) {}

    explicit Person(int idx) : name(nameFromIdx(idx)) {}

    // 编译器生成
    /* Person(const Person& rhs); */
    /* Person(Person&& rhs); */

private:
    string name;
};

void t3() {
    Person p("Nancy");
    /* auto cloneOfP(p); */
    // 通不过编译, 看起来是在调用拷贝构造函数,
    // 但是实际上调用的是完美转发构造函数,
    // 该函数尝试从一个Person类型对象(p)出发初始化另一个
    // Person类型对象的string性别的数据成员
    // 而string不具备接受Person型别形参的构造函数,

    // 模板实例化之后, 不含有const 饰词, 所以就是更佳的匹配

    // 仅有常量饰词可以使得拷贝构造调用成功:
    // 另一条规则: 如果在函数调用时,
    // 一个模板实例化函数和一个非函数模板(常规函数)具备相等的匹配程度,
    // 则先调用常规函数
    // 并且, 成员函数模板的具现化生成的特种成员函数
    // 并不会阻止编译器生成拷贝构造函数, 所以签名相同时,
    // 拷贝构造函数会呀过实例化的函数模板
    const Person cp("1");
    auto cloneOfP(cp);
}


// 派生列的拷贝和移动操作的平凡实现会表现出奇怪的行为
class SpecialPerson : public Person {
public:
    // 无论是拷贝构造函数还是移动构造函数, 调用的都是基类的完美转发构造函数
    /* SpecialPerson(const SpecialPerson& rhs) : Person(rhs) {} */
    /* SpecialPerson(SpecialPerson&& rhs) : Person(std::move(rhs)) {} */
    // 所以一定要避免把万能引用类型作为重载函数的形参选项
};

int main(int argc, char* argv[]) {
    /* t1(); */
    t2();

    return 0;
}
