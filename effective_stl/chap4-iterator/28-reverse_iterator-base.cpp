#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

vector<int> v{1, 2, 3, 4, 5};
vector<int>::reverse_iterator ri = find(v.rbegin(), v.rend(), 3);
vector<int>::iterator i(ri.base());

ostream &operator<<(ostream &os, const vector<int> &v) {
    for (auto i : v) os << i << " ";
    return os << endl;
}
void t1() {
    cout << v;
    cout << "*i=" << *i << endl;   // 4
    cout << "*ri=" << *ri << endl; // 3
    // insert
    v.insert(ri.base(), 99);
    cout << v;
}

void t2() {
    v.erase(--ri.base());
    // v.erase((++ri).base());
    cout << v;
}


int main(int argc, char const *argv[]) {
    // t1();
    t2();
    return 0;
}
