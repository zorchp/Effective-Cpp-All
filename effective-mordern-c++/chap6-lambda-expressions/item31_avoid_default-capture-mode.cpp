#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

void t1() {
    //
    int x{};
    auto f1 = [x](int y) { return x * y > 55; };
    auto f2 = f1;
    auto f3 = f2;
    f3(1);
}

using FilterContainer = vector<function<bool(int)>>;
FilterContainer filters;

// 如果在运行期才去计算出来5这个值, 那就要像下面这样:
auto computerDivisor = []() { return 5; };

void add1() {
    auto divisor = computerDivisor();

    // 这就可能导致空悬引用
    filters.emplace_back([&](int val) { return val % divisor == 0; });
    // 问题依然存在
    filters.emplace_back([&divisor](int val) { return val % divisor == 0; });
    // 按值传递就会好些
    filters.emplace_back([=](int val) { return val % divisor == 0; });
}

void t2() {
    filters.emplace_back([](int val) { return val % 5 == 0; });
    add1();
}
void t21() {
    for (auto i : filters) cout << i(1) << endl;
}

template <typename C>
void workWithContainer(const C &container) {
    auto divisor = computerDivisor();
    using ContElemT = typename C::value_type;
    if (all_of(begin(container), end(container),
               [&](const ContElemT &val) { return val % divisor == 0; }))
        cout << "1\n ";
}
// C++14
template <typename C>
void workWithContainer14(const C &container) {
    auto divisor = computerDivisor();
    if (all_of(begin(container), end(container),
               [&](const auto &val) { return val % divisor == 0; }))
        cout << "1\n ";
}

class Widget {
public:
    void addFilter() const;
    void addFilter_copy_local() const;
    void addFilter_copy_generlized_lambda() const;

private:
    int divisor;
};
void Widget::addFilter() const {
    // 仍然不安全
    filters.emplace_back([=](int val) { return val % divisor == 0; });
    // 这就导致捕获divisor失败
    /* filters.emplace_back([](int val) { return val % divisor == 0; }); */
    /* filters.emplace_back([divisor](int val) { return val % divisor == 0; });
     */
    /* filters.emplace_back([&divisor](int val) { return val % divisor == 0; });
     */
    // 错误的关键在于裸指针(this)的应用
}

void t3() {
    auto pw = make_unique<Widget>();
    pw->addFilter();
} // Widget 被销毁, filters持有空悬指针

void t31() {
    for (auto i : filters) i(1);
    //==24739==ERROR: AddressSanitizer: heap-use-after-free on address
    // 0x000103b00710 at pc 0x000100d46ed8 bp 0x00016f0fe460 sp 0x00016f0fe458
}

// 一个可能的解决方案:
void Widget::addFilter_copy_local() const {
    // 拷贝为局部变量
    auto divisor_copy = divisor;
    filters.emplace_back(
        [divisor_copy](int val) { return val % divisor_copy == 0; });
    // 按值的默认捕获也成立
    filters.emplace_back([=](int val) { return val % divisor_copy == 0; });
}
void t4() {
    //
    Widget w;
    w.addFilter_copy_local();
    cout << filters.size() << endl; // 2
    for (auto i : filters) {
        cout << i(1) << endl; // 0
    }
}

void Widget::addFilter_copy_generlized_lambda() const {
    filters.emplace_back(
        [divisor = divisor](int val) { return val % divisor == 0; });
}

void t5() {
    // C++14 有一种方法: 广义lambda捕获
    //  拷贝值然后执行
    Widget w;
    w.addFilter_copy_generlized_lambda();
    cout << filters.size() << endl;              // 1
    for (auto i : filters) cout << i(1) << endl; // 0
}

void addDivisorFilter() {
    static auto divisor = computerDivisor();
    filters.emplace_back([=](int val) { return val % divisor == 0; });
    ++divisor;
    cout << divisor << endl;
}

void t6() {
    // 多次调用addDivisorFilter, 实际上修改的是一份divisor, 所以也会出问题:
    addDivisorFilter();
    addDivisorFilter();
    addDivisorFilter();
    addDivisorFilter();
    addDivisorFilter();
    addDivisorFilter();
    addDivisorFilter();
    for (auto i : filters) cout << i(1) << endl;
    /* 6 */
    /* 7 */
    /* 8 */
    /* 9 */
    /* 10 */
    /* 11 */
    /* 12 */
    /* 0 */
    /* 0 */
    /* 0 */
    /* 0 */
    /* 0 */
    /* 0 */
    /* 0 */
}

int main(int argc, char *argv[]) {
    /* t1(); */

    /* t2(); */
    /* t21(); */

    /* t3(); */
    /* t31(); */

    /* t4(); */
    /* t5(); */
    t6();
    return 0;
}
