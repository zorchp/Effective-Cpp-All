#include <iostream>

using namespace std;

void t1() {
#define f(a, b) ((a > b) ? (a) : (b))

    // 使用宏
    int a = 5, b = 0;
    cout << f(++a, b) << endl;      // 7
    cout << f(++a, b + 10) << endl; // 10
}

/* clang++ -E %
void t1() {
    int a = 5, b = 0;
    cout << ((++a > b) ? (++a) : (b)) << endl;
    cout << ((++a > b + 10) ? (++a) : (b + 10)) << endl;
}
*/

template <typename T>
inline T f1(const T& a, const T& b) {
    return a > b ? a : b;
}

void t2() {
    int a = 5, b{};
    cout << f1(++a, b) << endl;      // 6
    cout << f1(++a, b + 10) << endl; // 10
}

int main(int argc, char const* argv[]) {
    // t1();
    t2();
    return 0;
}
