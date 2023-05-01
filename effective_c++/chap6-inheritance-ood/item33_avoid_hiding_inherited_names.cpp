#include <iostream>

using namespace std;

class Base {
private:
    int x;

public:
    virtual void mf1() = 0;
    virtual void mf1(int) { cout << "virtual void mf1(int)\n"; }
    virtual void mf2() { cout << "virtual void mf2()\n"; }
    void mf3() { cout << "Base void mf3()\n"; }
    void mf3(double) { cout << "void mf3(double)\n"; }
};

class Derived : private Base {
public:
    // 加上using声明, 就可以使用了
    /* using Base::mf1; */
    /* using Base::mf3; */
    virtual void mf1() {
        Base::mf1();
        cout << "virtual void mf1()\n";
    }
    void mf3() { cout << "Derived void mf3()\n"; }
    void mf4() { cout << "void mf4()\n"; }
};

void t1() {
    Derived d;
    int x;
    d.mf1(); // virtual void mf1()

    /* d.mf1(x); // error, Derived::mf1遮盖了Base::mf1 */

    /* d.mf2(); // virtual void mf2() */

    d.mf3(); // Derived void mf3()

    /* d.mf3(x); // error, Derived::mf3遮盖了Base::mf3 */
}

void t2() {
    Derived d;
    int x;
    d.mf1(); // virtual void mf1()

    /* d.mf1(x); // error, Derived::mf1遮盖了Base::mf1 */
}

int main(int argc, char *argv[]) {
    /* t1(); */
    t2();
    return 0;
}
