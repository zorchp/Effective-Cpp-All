#include <iostream>

using namespace std;

template <class T>
class SmartPtr {
public:
    SmartPtr(T *realPtr = 0) : pointee(realPtr) {}
    SmartPtr(const SmartPtr<T> &rhs) {
        pointee = rhs.pointee;
        rhs.pointee = 0;
    }
    ~SmartPtr() { delete pointee; }

    SmartPtr &operator=(const SmartPtr<T> &rhs) {
        if (this == &rhs) return *this;
        delete pointee;
        pointee = rhs.pointee;
        rhs.pointee = 0;
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

void t1() {
    //
}

int main(int argc, char *argv[]) {
    t1();
    return 0;
}
