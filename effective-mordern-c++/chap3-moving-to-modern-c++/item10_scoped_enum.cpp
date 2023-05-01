#include <type_traits> // underlying_type
#include <cstddef>
#include <cstdint> //uint64_t
#include <vector>
#include <iostream>

using namespace std;

void t1() {
    enum Color { black, white, red };
    /* auto white = false; */
    // Redefinition of 'white' as different kind of symbol
}

void t2() {
    // 第一个好处, 作用域封闭, 避免名字空间污染
    enum class Color { black, white, red };
    auto white = false;
    // Cannot initialize a variable of type 'Color' with an lvalue of type
    // 'bool'
    /* Color c0 = white; */
    Color c1 = Color::white;
    auto c2 = Color::white;
}

vector<size_t> primeFactors(size_t x) { return {1, 2, 3}; }

void t3() {
    // weird
    enum Color { black, white, red };
    // 强类型枚举不存在这个问题
    // 第二个好处, 不存在类型转换(到浮点类型或者整型)
    /* enum class Color { black, white, red }; */
    Color c = red;
    if (c < 14.5) {
        auto factors = primeFactors(c);
        for (auto i : factors) cout << i << " "; // 1 2 3
    }
    cout << endl;
    // 或者可以这样做强制类型转换
    if (static_cast<double>(c) < 14.5) {
        auto factors = primeFactors(static_cast<size_t>(c));
        for (auto i : factors) cout << i << " "; // 1 2 3
    }
    cout << endl;
}

// 第三个好处, 可以前置声明
enum class Color1;
/* enum Color; */ // 不行
void t4() {
    enum Status {
        good = 0,
        failed = 1,
        incomplete = 100,
        corrupt = 200, // add this
        indeterminate = 0xffffffff
    } e1;
    cout << sizeof(e1) << endl; // 4
}

void t5() {
    // 前置声明的好处, 降低编译依赖性
    enum class Status; // 默认型别是int, 但是可以修改
    enum class Status_uint32 : std::uint64_t;
}

// 使用弱类型枚举的好处, 可以为整型等无意义参数指定名称
// 以tuple为例
using UserInfo = tuple<string, string, size_t>;

void t6() {
    UserInfo uInfo{"1"s, "2"s, 10};
    auto val = get<1>(uInfo); // 不知道名称, 还需要知道源码
    cout << val << endl;      // 2
}

void t7() {
    // 使用弱类型枚举, 可以提供类型注释
    enum UserInfoFields { uiName, uiEmail, uiReputation };
    UserInfo uInfo{"1"s, "2"s, 10};
    auto val = get<uiReputation>(uInfo); // 用注释一目了然
    cout << val << endl;                 // 10
}

void t8() {
    // 强类型枚举可以做, 但是显得复杂
    enum class UserInfoFields { uiName, uiEmail, uiReputation };
    UserInfo uInfo{"111"s, "222"s, 123};
    auto val = get<static_cast<size_t>(UserInfoFields::uiEmail)>(uInfo);
    // 不转换类型就不行
    /* auto val = get<UserInfoFields::uiEmail>(uInfo); */
    cout << val << endl; // 222
}
// 当然可以通过模板函数简化:(C++11)
template <typename E>
constexpr typename underlying_type<E>::type toUType(E enumerator) noexcept {
    return static_cast<typename underlying_type<E>::type>(enumerator);
}

// C++14版更简化了:(返回值auto, 以及_t类型)
template <typename E>
/* constexpr typename underlying_type_t<E>::type toUType1(E enumerator) noexcept
   { */
constexpr auto toUType1(E enumerator) noexcept {
    return static_cast<underlying_type_t<E>>(enumerator);
}

void t9() {
    // 那么调用也变得方便了:(虽然还是不如弱类型枚举)
    // 但是可以避免污染命名空间和无意进行的涉及枚举类型的隐式类别转换
    enum class UserInfoFields { uiName, uiEmail, uiReputation };
    UserInfo uInfo{"111"s, "222"s, 123};
    auto val = get<toUType(UserInfoFields::uiEmail)>(uInfo);
    auto val1 = get<toUType1(UserInfoFields::uiEmail)>(uInfo);

    cout << val << endl;  // 222
    cout << val1 << endl; // 222
}

int main(int argc, char *argv[]) {
    /* t3(); */
    /* t4(); */
    /* t6(); */
    /* t7(); */
    /* t8(); */
    t9();
    return 0;
}
