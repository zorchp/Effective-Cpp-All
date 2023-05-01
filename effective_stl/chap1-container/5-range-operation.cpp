#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

template <typename T, template <typename> class C>
ostream &operator<<(ostream &os, const C<T> &v) {
    for (T i : v) os << i << " ";
    return os << endl;
}

void t1() {
    // 区间创建
    vector<int> v1{1, 2, 3, 4, 5};
    // 构造函数层面
    vector<int> v2(v1.begin() + 1, v1.begin() + 4);
    cout << v2; // 2 3 4
}

void t2() {
    // 区间插入
    vector<int> v1{1, 2, 3, 4, 5};
    vector<int> v2{10, 11, 12};
    // 成员函数层面
    v1.insert(v1.begin() + 1, v2.begin(), v2.end());
    cout << v1; // 1 10 11 12 2 3 4 5
}

void t3_1() {
    // 区间删除: 序列式容器
    vector<int> v1{1, 2, 3, 4, 5};
    v1.erase(v1.begin() + 1, v1.begin() + 4);
    cout << v1; // 1 5
}

void t3_2() {
    // 区间删除: 序列式容器
    set<int> s1{1, 2, 3, 4, 5};
    set<int>::const_iterator first = s1.find(1), last = s1.find(3);
    s1.erase(first, last); // erase: [first, last)
    cout << s1;            // 3 4 5
}

void t4() {
    // 区间赋值
    vector<int> v1{1, 2, 3, 4, 5};
    vector<int> v2{10, 11, 12};
    // 成员函数层面
    v1.assign(v2.begin(), v2.end() - 1);
    cout << v1; // 10 11

    v1.assign({9, 8, 7}); // 初始化列表
    cout << v1;           // 9 8 7

    v1.assign(10, 9); // count 个 value
    cout << v1;       // 9 9 9 9 9 9 9 9 9 9
}


int main(int argc, char const *argv[]) {
    // t1();
    // t2();
    // t3_1();
    // t3_2();
    t4();
    return 0;
}
