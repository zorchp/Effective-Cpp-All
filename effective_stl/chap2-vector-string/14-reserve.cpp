#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace chrono;
const int SIZE = 100000000;


void t1() {
    vector<int> v;
    for (int i = 0; i < SIZE; i++) v.push_back(i);
    cout << v.capacity() << endl;
    cout << v.size() << endl;
}

void t2() {
    vector<int> v;
    v.reserve(SIZE);
    for (int i = 0; i < SIZE; i++) v.push_back(i);
    cout << v.capacity() << endl;
    cout << v.size() << endl;
}

void time_used(void (*t)(void)) {
    auto start = system_clock::now();
    t();
    auto end = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "花费了"
         << double(duration.count()) * microseconds::period::num /
                microseconds::period::den
         << "秒" << endl;
}

int main(int argc, char* argv[]) {
    time_used(t1);
    cout << "---\n";
    time_used(t2);
    return 0;
}

/* 134217728 */
/* 100000000 */
/* 花费了1.3347秒 */
/* --- */
/* 100000000 */
/* 100000000 */
/* 花费了1.19844秒 */
/* ./14-reserve.out  2.20s user 0.32s system 99% cpu 2.544 total */

/* 134217728 */
/* 100000000 */
/* 花费了14.843秒 */
/* --- */
/* 100000000 */
/* 100000000 */
/* 花费了12.7968秒 */
/* ./14-reserve.out  27.34s user 0.58s system 98% cpu 28.300 total */
/*  */
/* [Process exited 0] */
