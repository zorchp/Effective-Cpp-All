#include <iostream>
using namespace std;

class P {
public:
    P() = default;

private:
    P(const P&);
    P& operator=(const P&);
};

void t1() {
    [[maybe_unused]] P p;
    /* P q(p); // Calling a private constructor of class 'P' */
}

class HomeForSale {
public:
    //
    HomeForSale() {}

private:
    // 这样可以阻止编译器自动生成copy-ctor和copy-assignment,
    // 但是(可能)会导致链接期错误
    HomeForSale(const HomeForSale&);
    HomeForSale& operator=(const HomeForSale&);
};

void t2() {
    [[maybe_unused]] HomeForSale h1, h2;
    /* h2(h1); // Type 'HomeForSale' does not provide a call operator */
}

class Uncopyable {
protected:
    Uncopyable() {}
    ~Uncopyable() {}

private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};

class HomeForSale1 : private Uncopyable {
public:
    //
    HomeForSale1() {}

private:
    // 这样可以阻止编译器自动生成copy-ctor和copy-assignment,
    // 但是(可能)会导致链接期错误
    HomeForSale1(const HomeForSale1&);
    HomeForSale1& operator=(const HomeForSale1&);
};

void t3() {
    [[maybe_unused]] HomeForSale h1, h2;
    /* h2(h1); // Type 'HomeForSale' does not provide a call operator */
}

int main(int argc, char* argv[]) {
    /* t1(); */
    /* t2(); */
    t3();
    return 0;
}
