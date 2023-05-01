#include <iostream>
#include <string>
using namespace std;

class Name {
public:
    Name(const string &s);
};

class Rational {
public:
    Rational(int numerator = 0, int denominator = 1)
        : n(numerator), d(denominator) {}
    operator double() const { return n * 1. / d; }

private:
    int n, d;
};

void t1() {
    Rational r(1, 2);
    double d = 0.5 * r;
    cout << d << endl;
}

template <class T>
class Array {
public:
    // 代理类, 仅用于表现即将被产生的数组的大小
    class ArraySize {
    public:
        ArraySize(int numElements) : theSize(numElements) {}
        int size() const { return theSize; }

        T &operator[](int index);

    private:
        int theSize;
    };

    Array(int lowBound, int highBound);
    // 单自变量ctor, 接受代理类对象
    Array(ArraySize size);

private:
    int data[];
};

bool operator==(const Array<int> &lhs, const Array<int> &rhs) {
    return true; // just for test
}

void t2() {
    Array<int> a1(10), b1(10);
    /* for (int i{}; i < 10; ++i) */
    /*     if (a1 == b1[i]) cout << "a==b[i]\n"; // error */
}

int main(int argc, char *argv[]) {
    /* t1(); */
    t2();
    return 0;
}
