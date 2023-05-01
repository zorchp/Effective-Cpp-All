#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <ostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace chrono;
const int SIZE = 100000000;

int trans(int x) { return x + 1; }

/* template <typename T> */
/* ostream &operator<<(ostream &os, vector<T> v) { */
/*     for (auto i : v) os << i << " "; */
/*     return os << endl; */
/* } */
/* template <typename T> */
/* ostream &operator<<(ostream &os, list<T> v) { */
/*     for (auto i : v) os << i << " "; */
/*     return os << endl; */
/* } */

// just for gcc
template <typename T, template <typename> class Container>
ostream &operator<<(ostream &os, Container<T> v) {
    for (auto i : v) os << i << " ";
    return os << endl;
}

void t1() {
    vector<int> vals{1, 3, 5}, res{0};
    cout << vals;
    /* transform(vals.begin(), vals.end(), res.end(), trans); */
    transform(vals.begin(), vals.end(), back_inserter(res), trans); // 0 2 4 6
    // just for list and deque
    /* transform(vals.begin(), vals.end(), front_inserter(res), trans); */

    cout << res;
}

void t2() {
    list<int> vals{1, 2, 3}, res{0};
    cout << vals;
    transform(vals.rbegin(), vals.rend(), front_inserter(res), trans);
    // 2 3 4 0
    cout << res;
}

void t3() {
    vector<int> vals{1, 2, 3}, res{0, 1, 2};
    cout << vals;
    transform(vals.rbegin(), vals.rend(),
              inserter(res, res.begin() + res.size() / 2), trans);
    // 0 4 3 2 1 2
    cout << res;
}
/* void time_used(void (*t)(void)) { */
/*     auto start = system_clock::now(); */
/*     t(); */
/*     auto end = system_clock::now(); */
/*     auto duration = duration_cast<microseconds>(end - start); */
/*     cout << "花费了" */
/*          << double(duration.count()) * microseconds::period::num / */
/*                 microseconds::period::den */
/*          << "秒" << endl; */
/* } */

int main(int argc, char *argv[]) {
    /* t1(); */
    /* t2(); */
    t3();
    return 0;
}
