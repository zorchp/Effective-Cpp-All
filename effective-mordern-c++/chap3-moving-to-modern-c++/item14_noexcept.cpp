#include <iostream>

using namespace std;

int f(int x) throw();  // C++98
int f(int x) noexcept; // C++11

// 为数组和pair准备的swap函数:
template <class T, size_t N>
void swap(T (&a)[N], T (&b)[N]) noexcept(noexcept(swap(*a, *b)));
/*
template <class T1, class T2>
struct pair {
    void swap(pair &p) noexcept(
        noexcept(swap(first, p.first)) &&noexcept(swap(second, p.second)));
};
*/


void t1() {
    //
}

int main(int argc, char *argv[]) {
    t1();
    return 0;
}
