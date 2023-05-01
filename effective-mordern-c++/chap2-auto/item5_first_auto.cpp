#include <iostream>
#include <iterator>
#include <memory>
#include <vector>
#include <unordered_map>
using namespace std;

template <typename It>
void dwim(It b, It e) { // do what I mean
    while (b != e) {
        /* typename iterator_traits<It>::value_type currVal = *b; */
        auto currVal = *b;
    }
}

void t1() {
    //
    int x;
    /* auto x1; */
    auto x2 = 1;
}

class Widget {
    int m_i;

public:
    operator int() { return m_i; }
};

// auto 推导的lambda函数
auto f = [](const unique_ptr<Widget>& p1, const unique_ptr<Widget>& p2) {
    return *p1 < *p2;
};
// c++14, auto lambda 函数
auto f1 = [](const auto& p1, const auto& p2) { return *p1 < *p2; };

// 传统的函数签名
bool g(const unique_ptr<Widget>&, const unique_ptr<Widget>&);

// function类模板, 函数相同, 但是由于需要实例化function模板,
// 导致占用内存比lambda(闭包函数)大
function<bool(const unique_ptr<Widget>&, const unique_ptr<Widget>&)> func;
function<bool(const unique_ptr<Widget>&, const unique_ptr<Widget>&)> h =
    [](const unique_ptr<Widget>& p1, const unique_ptr<Widget>& p2) {
        return *p1 < *p2;
    };

// 冗杂, 但是可以进行递归调用
function<bool(const unique_ptr<Widget>&, const unique_ptr<Widget>&)> h1 =
    [](const auto& p1, const auto& p2) { return *p1 < *p2; };

void t2() {
    vector<int> v{1, 2};
    // 可能存在32/64位机器的移植问题
    /* unsigned size = v.size(); */
    auto size = v.size();

    unordered_map<string, int> dic{};
    /* for (pair<string, int>& p : dic) { */
    /* for (pair<const string, int>& p : dic) { */
    for (auto& p : dic) { // best way
        //
        cout << p.first << endl;
        cout << p.second << endl;
    }
}

int main(int argc, char* argv[]) {
    /* t1(); */
    t2();
    return 0;
}
