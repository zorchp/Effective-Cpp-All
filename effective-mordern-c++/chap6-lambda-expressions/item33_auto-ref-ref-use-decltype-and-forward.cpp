#include <iostream>
#include <memory>

using namespace std;
// 对auto&& 型别的形参使用decltype, 以 std::forward 之

auto normalize(int x) { return x; }
auto func(int x) { return x; }

/* auto f = [](auto x) { return func(normalize(x)); }; */

// 类似于
class SomeCompilerGeneratedClassName {
public:
    template <typename T>
    auto operator()(T x) const {
        return func(normalize(x));
    }
};

// 需要考虑 normalize区别对待左值和右值的情况
auto f = [](auto &&x) { return func(normalize(std::forward<decltype(x)>(x))); };
auto g = [](auto &&...params) {
    return func(normalize(std::forward<decltype(params)>(params)...));
};

void t1() {
    //
    f(1);
}
int main(int argc, char *argv[]) {
    t1();
    return 0;
}
