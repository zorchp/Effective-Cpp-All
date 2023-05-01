#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <bitset>

using namespace std;

// 1. 不是STL容器
// 2. 不存储bool类型

void t1() {
    vector<int> vi{1, 0, 1, 0};
    cout << sizeof(vi) << endl; // 24
    int* pb = &vi[0];
    // cout << pb << endl;
}
void t2() {
    vector<bool> vb{true, false, false};
    cout << sizeof(vb) << endl; // 40

    //  不能取地址, 就意味着不能通过迭代器或指针修改其值
    //  bool* pb = &vb[0]; // error: taking address of rvalue
    //  // error: cannot convert 'std::vector<bool>::reference*' to 'bool*' in
    //  // initialization
    for (auto i : vb) cout << i << endl;
}
void t3() {
    // good way: use deque<bool>
    deque<bool> db{true, false, false};
    bool* dp = &db[0];
    cout << dp << endl; // 0x146e04560
    // 并且支持插入和删除等操作, 就是资源占用比较大
}

void t4() {
    bitset<2> bs(3); // 编译时指定
    // 非STL容器, 仅是C++标准的一部分, 不支持插入和删除,但是资源占用小
    cout << bs[0] << endl;
}

int main(int argc, char const* argv[]) {
    t1();
    t2();
    // t3();
    // t4();
    return 0;
}