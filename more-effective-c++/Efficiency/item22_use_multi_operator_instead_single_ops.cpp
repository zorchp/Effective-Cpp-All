
#include <iostream>
#include <ostream>
#include <vector>

using namespace std;

class Rational {
public:
    Rational(int numerator = 0, int denominator = 1)
        : n(numerator), d(denominator) {}
    int numerator() const { return n; }
    int denominator() const { return d; }
    Rational& operator+=(const Rational& rhs) {
        this->n = n * rhs.d + d * rhs.n;
        this->d = d * rhs.d;
        return *this;
    }
    Rational& operator-=(const Rational& rhs) {
        this->n = n * rhs.d - d * rhs.n;
        this->d = d * rhs.d;
        return *this;
    }

private:
    int n, d;
};

ostream& operator<<(ostream& os, const Rational& r) {
    return os << r.numerator() << " / " << r.denominator() << endl;
}
// 效率方面: 独身的操作符要比复合操作符效率低, 因为复合的返回引用,
// 仅对自身对象操作, 独身操作符需要返回一个临时对象, 而对象的构造需要资源

// Effective-C++ item24: 若所有参数都是都需隐式类型转换, 为此采用non-member-func
const Rational operator*(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.numerator() * rhs.numerator(),
                    lhs.denominator() * rhs.denominator());
}
// 用+=, -=实现+, -
template <class T>
const T operator+(const T& lhs, const T& rhs) {
    // T()是拷贝构造函数, 返回一个临时对象, 其值与lhs相同
    /* return T(lhs) += rhs; */
    // 下面这样写也可以, 但是就不能用返回值优化了
    T result(lhs); // 产生具名对象
    return result += rhs;
}

template <class T>
const T operator-(const T& lhs, const T& rhs) {
    return T(lhs) -= rhs;
}

void t1() {
    //
    Rational a(1, 3), b(1, 2);
    Rational c = a + b;
    Rational d = a - b;
    cout << c;
    cout << d;
    /* 5 / 6 */
    /* -1 / 6 */
}

int main(int argc, char* argv[]) {
    t1();
    return 0;
}
