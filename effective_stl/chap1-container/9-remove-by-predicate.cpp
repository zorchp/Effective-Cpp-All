#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <set>
#include <algorithm>
#include <fstream>


using namespace std;

template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v) {
    for (T i : v) os << i << " ";
    return os << endl;
}

template <typename T>
ostream &operator<<(ostream &os, const list<T> &v) {
    for (T i : v) os << i << " ";
    return os << endl;
}

template <typename T>
ostream &operator<<(ostream &os, const set<T> &v) {
    for (T i : v) os << i << " ";
    return os << endl;
}

bool badValue(int x) { return x < 5; }

void t1() {
    vector<int> v{1, 2, 1, 3, 4, 5, 9};
    // best way for vector, deque, string and list
    v.erase(remove_if(v.begin(), v.end(), badValue), v.end());
    cout << v; // 5 9
}

void t2() {
    list<int> l{1, 2, 3, 10, 12};
    // best way: `list::remove_if()` just for list
    l.remove_if(badValue);
    cout << l; // 10 12
}

void t3() {
    // maybe a good way for associative container: copy new object
    set<int> s{11, 2, 13, 4};
    set<int> goodValues{};
    remove_copy_if(s.begin(), s.end(), inserter(goodValues, goodValues.end()),
                   badValue);
    cout << goodValues; // 11 13
}

void t4() {
    // remove from associative container by for-loop
    set<int> s{11, 2, 13, 4};
    for (auto it = s.begin(); it != s.end(); ++it)
        // 未定义的行为: 删除一个元素的时候, 对应的迭代器失效了
        if (badValue(*it)) s.erase(it); // segmentation fault
    cout << s;
}

void t5() {
    set<int> s{11, 2, 13, 4};
    // 上面方法的改进
    for (auto it = s.begin(); it != s.end();)
        if (badValue(*it))
            s.erase(it++);
        else
            ++it;
    cout << s; // 11 13
}

void t6() {
    ofstream logFile;
    set<int> s{11, 2, 13, 4};
    // 关联式容器, 删除时候加一条日志信息
    for (auto it = s.begin(); it != s.end();)
        if (badValue(*it)) {
            // 只需要在这里 加日志信息
            logFile << "Erasing" << *it << "\n";
            s.erase(it++);
        } else
            ++it;
    cout << s; // 11 13
}

void t7() {
    ofstream logFile;
    vector<int> v{1, 2, 1, 3, 4, 5, 9};
    for (auto it = v.begin(); it != v.end();)
        if (badValue(*it)) {
            logFile << "Erasing " << *it << '\n';
            it = v.erase(it); // 返回迭代器, 使之一直有效
        } else
            ++it;
    cout << v; // 5 9
}

int main(int argc, char const *argv[]) {
    // t1();
    // t2();
    // t3();
    // t4();
    // t5();
    // t6();
    t7();

    return 0;
}