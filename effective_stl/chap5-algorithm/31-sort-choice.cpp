#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <functional>


using namespace std;

struct P {
    P(int i) : age(i) {}
    void get() { cout << age << " "; }
    bool operator==(const P &rhs) const { return age == rhs.age; }
    bool operator<(const P &rhs) const { return age < rhs.age; }
    int age;
};


vector<P> vp{1, 3, 2, 9, 12, 4, 31, 49, 30};

void t1() {
    // 对n个满足条件的元素排序
    partial_sort(vp.begin(), vp.begin() + 5, vp.end(), less<P>());
    for (auto i : vp) cout << i.age << " "; // 1 2 3 4 9 12 31 49 30
    cout << endl;
}
void t2() {
    // 不要求顺序, 只要选前n个满足条件的元素
    nth_element(vp.begin(), vp.begin() + 5, vp.end(), less<P>());
    for (auto i : vp) cout << i.age << " "; // 1 3 2 9 4 12 31 49 30
    cout << endl;
}

void t3() {
    vector<P>::iterator begin(vp.begin());
    vector<P>::iterator end(vp.end());
    vector<P>::iterator pos;
    // 找中间
    pos = begin + vp.size() / 2;
    nth_element(begin, pos, end, less<P>());
    cout << pos->age << endl; // 9
    // 找75%
    pos = begin + vp.size() * 0.25;
    nth_element(begin, pos, end, less<P>());
    cout << pos->age << endl; // 3
}


void t4() {
    // 按照比较规则(传入的函数), 将原始区间分成两部分,
    // pos位置(含)往后就是不满足条件的元素;
    auto goodEnd =
        partition(vp.begin(), vp.end(), [](const P &p) { return p.age >= 6; });
    cout << goodEnd->age << endl;           // 4
    for (auto i : vp) cout << i.age << " "; // 30 49 31 9 12 4 2 3 1

    cout << endl;
}
/* 小结
1. 如果需要值相等元素的相对顺序在排序后保持不变,
那么就要使用stable_sort或者stable_partition
2. 全局的排序算法只针对具有随机访问迭代器的容器, 即vector,deque,string,array,
对于list, 具有成员函数sort(是稳定排序)
3. 对关联式容器, map, set 都具有排序的特性(红黑树实现)
4. 性能方面,
partition > stable_partition > nth_element > partial_sort > sort > stable_sort
5. priority_queue不是标准STL容器(因为不支持迭代器), 但是也具有排序特性.
*/

int main(int argc, char *argv[]) {
    /* t1(); */
    /* t2(); */
    /* t3(); */
    t4();
    return 0;
}
