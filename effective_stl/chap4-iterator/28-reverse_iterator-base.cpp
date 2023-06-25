#include <iterator>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

vector<int> v{1, 2, 3, 4, 5};
vector<int>::reverse_iterator ri = find(v.rbegin(), v.rend(), 3);
vector<int>::iterator i(ri.base());

ostream &operator<<(ostream &os, const vector<int> &v) {
    for (auto i : v)
        os << i << " ";
    return os << endl;
}
void t1() {
    cout << v;
    cout << "*i=" << *i << endl;   // 4
    cout << "*ri=" << *ri << endl; // 3
    // cout << *v.rbegin();           // 5
    // insert
    v.insert(ri.base(), 99);
    cout << v;
    // 1 2 3 4 5
    // *i=4
    // *ri=3
    // 1 2 3 99 4 5
}

void t2() {
    // v.erase(--ri.base());
    v.erase((++ri).base()); // best way
    cout << v;
    // 1 2 4 5
}

void t3() {
    // ref:
    // https://stackoverflow.com/questions/1830158/how-to-call-erase-with-a-reverse-iterator
    v.erase(std::next(ri).base()); // don't change ri
    // advance(ri, 1);
    // v.erase(ri.base());
    cout << v;
    // 1 2 4 5
}


int main(int argc, char const *argv[]) {
    // t1();
    // t2();
    t3();
    return 0;
}
