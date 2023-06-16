#include <iostream>
using namespace std;

template <class T>
class SmartPtr {
public:
    explicit SmartPtr(T *realPtr = nullptr) : pointee(realPtr) {}

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
    T *operator->() const { return pointee; }
    T &operator*() const { return *pointee; }
    T *get() const { return pointee; }

    T *release() { // 转换为普通指针
        T *oldPtr = pointee;
        pointee = nullptr;
        return oldPtr;
    }
    void reset(T *p = nullptr) { // 重置为空指针
        if (pointee != p) {
            delete pointee;
            pointee = p;
        }
    }

private:
    T *pointee;
};


class P {
public:
    P() { cout << __FUNCTION__ << '\n'; }
    void f() { //
        cout << __FUNCTION__ << '\n';
    }
    ~P() { cout << __FUNCTION__ << '\n'; }
};

void t1() {
    //
    auto p = SmartPtr<P>(new P);
    p->f();
    auto q = SmartPtr<P>(new P);
    q = p;
    q->f();
    auto r = q.release();
    r->f();
}

void t2() {
    //
}

int main(int argc, char *argv[]) {
    t1();
    // t2();
    return 0;
}
