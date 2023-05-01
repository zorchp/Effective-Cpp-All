#include <iostream>

using namespace std;

class B {
public:
    virtual void f() const { cout << "B::f\n"; }
};

// 并未实现多态, 子类将基类的f()覆盖了
class D : public B {
public:
    // warning: 'D::f' hides overloaded virtual function [-Woverloaded-virtual]
    virtual void f() { cout << "D::f\n"; }
};

void t1() {
    D d1;
    d1.f(); // D::f
}

int main(int argc, char *argv[]) {
    t1();
    return 0;
}
