#include <vector>
#include <iostream>
#include <algorithm>
#include <list>

using namespace std;
vector<int> get_vec() {
    vector<int> v;
    v.reserve(10);

    for (int i{1}; i <= 10; ++i) v.emplace_back(i);
    v[3] = v[5] = v[9] = 99;
    return v;
}

void t1() {
    auto v = get_vec();
    for (auto i : v) cout << i << " ";
    cout << endl;

    auto it = remove(v.begin(), v.end(), 99);
    cout << "*it = " << *it << endl; // 8

    for (auto i : v) cout << i << " ";
    cout << endl;
    // 1 2 3 99 5 99 7 8 9 99
    // 1 2 3 5 7 8 9 8 9 99
    v.erase(it, v.end());
    for (auto i : v) cout << i << " ";
    cout << endl; // 1 2 3 5 7 8 9
}

void t2() {
    list<int> l{1, 2, 3, 4, 3, 6};
    for (auto i : l) cout << i << " ";
    cout << endl;
    l.remove(3); // 可以直接remove
    for (auto i : l) cout << i << " ";
    cout << endl;
    // 1 2 3 4 3 6
    // 1 2 4 6
}
// MyRemove: LeetCode27
// https://leetcode.cn/problems/remove-element/
using vi = vector<int>::iterator;

int myRemove(vector<int> &v, int val) {
    int l{};
    for (int i{}; i < v.size(); ++i)
        if (val != v[i]) v[l++] = v[i];
    return l;
}
void myErase(vector<int> &v, int idx) {
    // cout << idx << endl; // 7
    v.resize(idx);
}

void t3() {
    auto v = get_vec();
    for (auto i : v) cout << i << " ";
    cout << endl;
    auto it = myRemove(v, 99);
    for (auto i : v) cout << i << " ";
    cout << endl;
    // 1 2 3 99 5 99 7 8 9 99
    // 1 2 3 5 7 8 9 8 9 99
    myErase(v, it);
    for (auto i : v) cout << i << " ";
    cout << endl;
}


int main(int argc, char const *argv[]) {
    // t1();
    // t2();
    t3();
    return 0;
}