#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <set>
#include <algorithm>


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

void t1() {
    vector<int> v{1, 2, 1, 3, 4, 5, 9};
    // best way for vector, deque, string
    v.erase(remove(v.begin(), v.end(), 1), v.end());
    cout << v; // 2 3 4 5 9
}

void t2() {
    list<int> l{1, 2, 3, 1, 2};
    // best way: `list::remove()` just for list
    l.remove(1);
    cout << l; // 2 3 2
}

void t3() {
    // best way for assocaitive container
    set<int> s{1, 2, 3, 4};
    s.erase(2);
    cout << s; // 1 3 4
}

int main(int argc, char const *argv[]) {
    // t1();
    // t2();
    t3();
    return 0;
}