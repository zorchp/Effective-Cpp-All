#include <iostream>
#include <iterator>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <functional>

using namespace std;
/*
有一个容器，或者有一对迭代器标识了一个区间，现在你希望在容器或者区间中查找一些信息，这样的查找工作应该如何进行呢？
你的选择往往是：count、count_if、find、find_if、binary_search、lower_bound、upper_bound以及equal_range。

1.
如果区间是排序的，那么通过binary_search、lower_bound、upper_bound和equal_range，你可以获得更快的查找速度
2.
如果迭代器并没有指定一个排序的区间，那么你的选择范围将局限于count、count_if、find以及find_if，而这些算法仅能提供线性时间的效率。
*/

struct P {
    P(int i) : age(i) {}
    void get() { cout << age << " "; }
    bool operator==(const P &rhs) const { return age == rhs.age; }
    bool operator<(const P &rhs) const { return age < rhs.age; }
    int age;
};


void t1() {
    list<P> lp{1, 4, 2, 6, 4, 6};

    // 仅想知道一个list容器中是否存在特定的对象值, 使用count:
    P p(6);
    if (count(lp.begin(), lp.end(), p) != 0) cout << "found\n"; // found
    // 或者使用find, 但是写起来麻烦
    if (find(lp.begin(), lp.end(), p) != lp.end()) cout << "found\n"; // found
}


void t2() {
    vector<P> v{1, 3, 2, 4, 7, 9, 3, 5, 6};
    sort(v.begin(), v.end());
    P p(6);
    if (binary_search(v.begin(), v.end(), p))
        cout << "found\n"; // found
    else
        cout << "not found\n";
}

void t3() {
    vector<P> v{1, 3, 2, 4, 7, 9, 3, 5, 6};
    sort(v.begin(), v.end());
    P p(3);
    auto it = lower_bound(v.begin(), v.end(), p);
    if (it != v.end() && *it == p.age) cout << "found\n"; // found

    using vpi = vector<P>::iterator;

    pair<vpi, vpi> pa = equal_range(v.begin(), v.end(), p);
    auto [a, b] = equal_range(v.begin(), v.end(), p);
    if (a != b) cout << "found\n"; // found

    cout << a->age << endl;                           // 3
    cout << b->age << endl;                           // 4
    cout << "count of 3: " << distance(a, b) << endl; // 2
}

void t4() {
    vector<P> v{1, 3, 2, 4, 7, 9, 3, 5, 6};
    sort(v.begin(), v.end());
    for (auto i : v) cout << i.age << " ";
    v.erase(v.begin(), lower_bound(v.begin(), v.end(), P(3)));
    cout << endl;
    for (auto i : v) cout << i.age << " ";
    cout << endl;
    /* 1 2 3 3 4 5 6 7 9  */
    /* 3 3 4 5 6 7 9  */
}

void t5() {
    vector<P> v{1, 3, 2, 4, 7, 9, 3, 5, 6};
    sort(v.begin(), v.end());
    for (auto i : v) cout << i.age << " ";
    v.erase(v.begin(), upper_bound(v.begin(), v.end(), P(3)));
    cout << endl;
    for (auto i : v) cout << i.age << " ";
    cout << endl;
    /* 1 2 3 3 4 5 6 7 9  */
    /* 4 5 6 7 9  */
}

void t6() {
    list<P> lp{1, 3, 2, 4, 7, 9, 3, 5, 6};
    /* sort(v.begin(), v.end()); */
    lp.sort(less<P>());
    for (auto i : lp) cout << i.age << " ";
    cout << endl;
    lp.insert(upper_bound(lp.begin(), lp.end(), P(8), less<P>()), P(8));
    for (auto i : lp) cout << i.age << " ";
    cout << endl;
    /* 1 2 3 3 4 5 6 7 9  */
    /* 1 2 3 3 4 5 6 7 8 9  */
}

int main(int argc, char const *argv[]) {
    // t1();
    // t2();
    /* t3(); */
    /* t4(); */
    /* t5(); */
    t6();
    return 0;
}
