#include <__functional/mem_fun_ref.h>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <new>
#include <sys/_types/_size_t.h>

using namespace std;

void OutOfMem() {
    cerr << "err\n";
    abort();
}

void t1() {
    set_new_handler(OutOfMem);
    int *p = new int[100000000000000L]; // err
}

// 为不同的类实现不同的new_handler
class X {
public:
    static void OutOfMem();
    static new_handler set_new_handler(new_handler p) throw();
    static void *operator new(size_t size) throw(bad_alloc);

private:
    static new_handler cur_handler;
};

new_handler X::cur_handler = 0;
// 同时也是标准库实现
new_handler X::set_new_handler(new_handler p) throw() {
    new_handler oldHandler = cur_handler;
    cur_handler = p;
    return oldHandler;
}

class NewHandlerHolder {
public:
    explicit NewHandlerHolder(new_handler nh) : handler(nh) {}

private:
    new_handler handler;
    NewHandlerHolder(const NewHandlerHolder &);
    NewHandlerHolder &operator=(const NewHandlerHolder);
};

void *X::operator new(size_t size) throw(bad_alloc) {
    NewHandlerHolder h(std::set_new_handler(cur_handler));
    return ::operator new(size);
}

class Y {
public:
    static void OutOfMem();
};

void t2() {
    // 分配X失败, 调用X::OutOfMem
    X *px = new X;
    // 分配Y失败, 调用Y::OutOfMem
    /* Y *py = new Y; */
}

void t3() {
    //
    X *px = new X;
    X::set_new_handler(0);
    /* X *px1 = new (nothrow) X; */
}

int main(int argc, char *argv[]) {
    /* t1(); */
    t3();
    return 0;
}
