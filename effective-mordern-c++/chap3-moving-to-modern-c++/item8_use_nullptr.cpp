#include <cstddef>
#include <iostream>
#include <memory>
#include <mutex>

using namespace std;

void f(int) { cout << "int\n"; }
void f(bool) { cout << "bool\n"; }
void f(void*) { cout << "void*\n"; }

void t1() {
    f(0);
    /* f(NULL); // Call to 'f' is ambiguous */
    f(nullptr);

    /* int */
    /* void* */
}

int findRecode(int i) { return i; }
void* findRecode(void* i) { return i; }

void t2() {
    auto res0 = findRecode(0);
    auto res1 = findRecode(nullptr);
    if (res1 == nullptr) cout << "nullptr\n";
}

// mutex test
class Widget {};
int f1(shared_ptr<Widget> spw) { return 1; }
double f2(unique_ptr<Widget> upw) { return 1.0; }
bool f3(Widget* pw) { return true; }

mutex f1m, f2m, f3m;
using MuxGuard = lock_guard<mutex>;

void t3() {
    MuxGuard g(f1m);
    auto res = f1(0);
}

void t4() {
    MuxGuard g(f2m);
    auto res = f2(NULL);
}
void t5() {
    MuxGuard g(f3m);
    auto res = f3(nullptr);
}
// 上面的设计是冗余的, 通过模版来做就简单很多
// C++11
template <typename FuncType, typename MuxType, typename PtrType>
auto lockAndCall(FuncType func, MuxType& mutex, PtrType ptr)
    -> decltype(func(ptr)) {
    MuxGuard g(mutex);
    return func(ptr);
}
// C++14
template <typename FuncType, typename MuxType, typename PtrType>
decltype(auto) lockAndCall_14(FuncType func, MuxType& mutex, PtrType ptr) {
    MuxGuard g(mutex);
    return func(ptr);
}

void t6() {
    /* auto res1 = lockAndCall(f1, f1m, 0); */
    /* auto res2 = lockAndCall(f2, f2m, NULL); */
    auto res3 = lockAndCall(f3, f3m, nullptr);
}

void t7() {
    /* auto res1 = lockAndCall_14(f1, f1m, 0); */
    /* auto res2 = lockAndCall_14(f2, f2m, NULL); */
    auto res3 = lockAndCall_14(f3, f3m, nullptr);
}

int main(int argc, char* argv[]) {
    /* t1(); */
    t6();
    t7();
    return 0;
}
