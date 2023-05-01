
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <type_traits>
#include <utility>

using namespace std;

// 为了避免26条提出的一些问题, 可以采用以下解决方案:
/*
1. 舍弃重载
2. 传递const T& 型别的形参
3. 传值(非引用)
4. 标签分派(*)
*/

multiset<string> names;


// 考虑一个重载版本: 从idx读取string
string nameFromIdx(int idx) { return to_string(idx); }

template <typename T>
void logAndAddImpl(T&& name, std::false_type) { // 针对非整型实参
    // get-current-time
    // log(now, "logAndAdd");

    names.emplace(std::forward<T>(name));
}

template <typename T>
void logAndAdd(T&& name) {

    /* logAndAddImpl(std::forward<T>(name), std::is_integral<T>()); */
    /* logAndAddImpl(std::forward<T>(name), std::is_integral<typename
     * std::remove_reference<T>::type>()); */
    // C++14
    logAndAddImpl(std::forward<T>(name),
                  // 重载决议
                  std::is_integral<std::remove_reference_t<T>>());
}

// 重载函数
void logAndAddImpl(int idx, std::true_type) { // 针对非整型实参
    logAndAdd(nameFromIdx(idx));
}

// 如果编译器会自行生成复制和移动构造函数, 那么仅撰写一个构造函数,
// 然后在其中运用标签分派,
// 那么有些针对构造函数的调用就可能会由编译器自动生成的构造函数接手处理,
// 从而绕过了标签分派系统


// 下面考虑采用SFINAE技巧以及 std::enable_if 限制接受万能引用实参的模板

class Person {
public:
    // SFINAE
    // ver1: 解决使用编译器生成的拷贝构造函数的问题
    /* template <typename T, */
    /*           typename = typename std::enable_if<!std::is_same< */
    /*               Person, typename std::decay<T>::type>::value>::type> */
    /* explicit Person(T&& n) : name(std::forward<T>(n)) {} */

    // ver2: 解决派生类调用基类的完美转发构造函数问题
    /* template <typename T, */
    /*           typename = typename std::enable_if<!std::is_base_of< */
    /*               Person, typename std::decay<T>::type>::value>::type> */
    /* explicit Person(T&& n) : name(std::forward<T>(n)) {} */

    // ver3: 使用C++14改进版,
    // 并加上对整型实参的处理(传入整型实参时禁用模板构造函数)
    template <typename T,
              typename = std::enable_if_t<
                  !std::is_base_of<Person, std::decay_t<T>>::value &&
                  !std::is_integral<std::remove_reference_t<T>>::value>>
    explicit Person(T&& n) : name(std::forward<T>(n)) {
        // 撰写静态断言: 验证转发函数的万能引用形参是否能作为初始化参数
        static_assert(std::is_constructible<string, T>::value,
                      "Parameter n can't be used to construct a std::string");
    }

    explicit Person(int idx) : name(nameFromIdx(idx)) {}
    // 回顾: 所有的工作:
    //  1. 为Person 添加一个处理整型实参的构造函数重载版本
    //  2. 进一步限制模板构造函数, 在接受整型实参时禁用之
    // 优势:
    //  1. 利用了完美转发, 达成了最高效率
    //  2. 控制了万能引用和重载的组合, 而非简单地禁用之
private:
    string name;
};

// 派生列的拷贝和移动操作的平凡实现会表现出奇怪的行为
class SpecialPerson : public Person {
public:
    // 无论是拷贝构造函数还是移动构造函数, 调用的都是基类的完美转发构造函数
    SpecialPerson(const SpecialPerson& rhs) : Person(rhs) {}
    SpecialPerson(SpecialPerson&& rhs) : Person(std::move(rhs)) {}
    // 所以一定要避免把万能引用类型作为重载函数的形参选项
};

void t1() {
    //
    Person p1("123");
    auto p2(p1);
    short a = 1;
    Person p3(a);
    /* Person p4(u"aub"); */
    // error: no matching function for call to
    // 'std::__cxx11::basic_string<char>::basic_string(const char16_t [4])'
}


int main(int argc, char* argv[]) {
    t1();
    return 0;
}
