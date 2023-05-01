#include <set>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <iostream>

using namespace std;

void t1() {
    set<int, less_equal<int>> s;
    s.insert(10);
    s.insert(10);
    for (auto i : s) cout << i << " "; // 10 10
}

struct DereferenceLess {
    template <typename T>
    bool operator()(T ps1, T ps2) const {
        return !(*ps1 < *ps2);
    }
};

void t2() {
    set<string *, DereferenceLess> s;
    s.insert(new string("10"));
    s.insert(new string("10"));
    for (auto i : s) cout << *i << " "; // 10 10
}

// for multiset
void t3() {
    multiset<int> ms;
    ms.insert(10);
    ms.insert(10);
    ms.insert(10);
    for (auto i : ms) cout << i << " ";
    cout << endl;
    auto p = ms.equal_range(10);
    // inequal
    if (p.first == p.second) cout << "equal\n";

    // 同一元素
    cout << *p.first << endl;  // 10
    cout << *p.second << endl; // 3
    // ms.erase(p.second);
    for (auto i : ms) cout << i << endl;
}
void t4() {
    multiset<int, less_equal<int>> ms;
    ms.insert(10);
    ms.insert(10);
    for (auto i : ms) cout << i << " ";
    cout << endl;
    auto p = ms.equal_range(10);
    // 说明不存在这一区间
    if (p.first == p.second) cout << "equal\n"; // equal
    cout << *(--p.first) << endl;
    // 同一元素
    // cout << *p.first << endl;  // 2
    // cout << *p.second << endl; // 2
}

int main(int argc, char const *argv[]) {
    // t1();
    // t2();
    // t3();
    t4();
    return 0;
}