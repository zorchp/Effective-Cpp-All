#include <deque>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <iterator>

#include <iostream>

using namespace std;

typedef deque<int> ID;
typedef ID::iterator IT;
typedef ID::const_iterator CIT;
ID d1{1, 2, 3, 4, 5, 6};

void t1() {

    IT i(d1.begin());
    CIT ci(d1.cbegin());
    // 此时存在隐式类型转换
    if (i == ci) cout << "c==ci\n"; // c==ci
}

void t2() {
    CIT ci(d1.cbegin() + 2);
    // error: no matching constructor for initialization of 'IT' (aka
    // '__deque_iterator<int, int *, int &, int **, long>')
    // IT i(ci);
}

void t3() {
    CIT ci(d1.cbegin() + 2);
    IT i(d1.begin());
    // advance(i, distance(i, ci)); // error
    // 需要指定类型, 强转为const
    advance(i, distance<CIT>(i, ci));
    cout << *i << endl;  // 3
    cout << *ci << endl; // 3
}

int main(int argc, char const *argv[]) {
    // t1();
    // t2();
    t3();
    return 0;
}