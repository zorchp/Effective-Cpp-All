#include <cstdio>
#include <cassert>
#include <iostream>
#include <ostream>

template <class T>
class auto_ptr {
public:
    explicit auto_ptr(T *realPtr = nullptr) : pointee(realPtr) {
    }

    auto_ptr(auto_ptr<T> &rhs) : pointee(rhs.release()) {
    }
    ~auto_ptr() {
        delete pointee;
    }

    auto_ptr &operator=(auto_ptr<T> &rhs) {
        if (this == &rhs)
            return *this;
        delete pointee;
        pointee = rhs.release();
        return *this;
    }
    T *operator->() const {
        return pointee;
    }
    T &operator*() const {
        return *pointee;
    }
    T *get() const {
        return pointee;
    }

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
    P() {
        printf("%s\n", __FUNCTION__);
    }
    void f() { //
        printf("%s\n", __FUNCTION__);
    }
    ~P() {
        printf("%s\n", __FUNCTION__);
    }
    int val;
};

void t1() {
    auto p = auto_ptr<P>(new P); // P
    p->f();                      // f
    auto q = auto_ptr<P>(new P); // P
    q = p;                       //~P
    q->f();                      // f
    auto r = q.release();
    r->f();   // f
    delete r; //~P
}

void t2() {
    //
    auto p = auto_ptr<P>(new P); // P
    p->f();                      // f
    auto q(p);
} //~P

void print(std::ostream &s, auto_ptr<P> p) {
    s << (*p).val << '\n';
}
void t3() {
    // 由于拷贝构造函数会转移对象的所有权, 所以如果传值会出现资源泄露
    auto_ptr<P> p(new P);
    p->val = 10;
    print(std::cout, p); // 值传递
    // 此时 p 为空指针了
    // assert(p.release() != nullptr);
    p->val = 10; // runtime error: member access within null pointer of type 'P'
}

void print1(std::ostream &s, const auto_ptr<P> &p) {
    s << (*p).val << '\n';
}
void t4() {
    // 使用常引用就可以了
    auto_ptr<P> p(new P);
    p->val = 10;
    print1(std::cout, p);
    p->val = 12;
    print1(std::cout, p);
} //~P

void t5() {
    std::vector<auto_ptr<P>> vp;
    auto_ptr<P> a(new P);
    vp.push_back(a); // 此时作为右值传入, 不会报错(C++11)
    // 如果是 C++98, 会因为拷贝构造函数出现问题
    // (后面调用时候指针实际上是失效的)
}

int main(int argc, char *argv[]) {
    // t1();
    // t2();
    // t3();
    // t4();
    t5();
    return 0;
}
