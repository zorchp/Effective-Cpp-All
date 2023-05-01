#include <set>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <iterator>
#include <iostream>

using namespace std;

void t1() {
    set<string *> ssp{}; // 实际上是下面这样
    // set<string *, less<string *>> ssp{};
    ssp.insert(new string("Ant"));
    ssp.insert(new string("Wom"));
    ssp.insert(new string("Lem"));
    ssp.insert(new string("Pen"));

    // 按照指针顺序, 而非字符串顺序
    for (set<string *>::const_iterator i = ssp.cbegin(); i != ssp.cend(); ++i)
        cout << *i << " " << **i << endl;
    // 0x600002e5d100 Ant
    // 0x600002e5d120 Wom
    // 0x600002e5d140 Lem
    // 0x600002e5d160 Pen
    copy(ssp.begin(), ssp.end(),
         ostream_iterator<string *>(cout, "\n")); // just ptr
}

// struct StringPtrLess : public function<bool(const string *, const string *)>
// {
struct StringPtrLess {
    bool operator()(const string *ps1, const string *ps2) const {
        return *ps1 < *ps2;
    }
};

struct DereferenceLess {
    template <typename T>
    bool operator()(T ps1, T ps2) const {
        return *ps1 < *ps2;
    }
};

void t2() {
    // 需要自定义比较函数(仿函数)
    // typedef set<string *, StringPtrLess> StringPtrSet;
    // 泛化
    typedef set<string *, DereferenceLess> StringPtrSet;
    StringPtrSet ssp;
    ssp.insert(new string("Ant"));
    ssp.insert(new string("Wom"));
    ssp.insert(new string("Lem"));
    ssp.insert(new string("Pen"));
    for (StringPtrSet::const_iterator i = ssp.cbegin(); i != ssp.cend(); ++i)
        cout << *i << " " << **i << endl; // right
    // 0x6000036f9100 Ant
    // 0x6000036f9140 Lem
    // 0x6000036f9160 Pen
    // 0x6000036f9120 Wom
    for_each(ssp.begin(), ssp.end(),
             [](const string *s) { cout << *s << " "; });
    cout << '\n';
    transform(ssp.begin(), ssp.end(), ostream_iterator<string>(cout, "\n"),
              [](const string *s) { return *s; });
    // Ant Lem Pen Wom
    // Ant
    // Lem
    // Pen
    // Wom
}

int main(int argc, char const *argv[]) {
    // t1();
    t2();
    return 0;
}