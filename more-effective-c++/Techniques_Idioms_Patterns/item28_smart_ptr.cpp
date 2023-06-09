#include <iostream>

using namespace std;

template <class T>
class SmartPtr {
public:
    SmartPtr(T *realPtr = nullptr) : pointee(realPtr) {}
    SmartPtr(SmartPtr<T> &rhs) {
        pointee = rhs.pointee;
        rhs.pointee = nullptr;
    }
    ~SmartPtr() { delete pointee; }

    SmartPtr &operator=(SmartPtr<T> &rhs) {
        if (this == &rhs) return *this;
        delete pointee;
        pointee = rhs.pointee;
        rhs.pointee = nullptr;
        return *this;
    }
    T *operator->() const {
        // process
        return pointee;
    }
    T &operator*() const {
        // process
        return *pointee;
    }

private:
    T *pointee;
};
class P {
public:
    P() { cout << __FUNCTION__ << '\n'; }
    void f() { cout << __FUNCTION__ << '\n'; }
    ~P() { cout << __FUNCTION__ << '\n'; }
};

void t1() {
    //
    auto p = SmartPtr<P>(new P);
    p->f();
    auto q = SmartPtr<P>(new P);
    q = p;
    q->f();
}

int main(int argc, char *argv[]) {
    t1();
    return 0;
}
