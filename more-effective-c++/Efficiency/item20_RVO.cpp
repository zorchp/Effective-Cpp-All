#include <iostream>
#include <ostream>
#include <vector>

using namespace std;

class Rational {
public:
    Rational(int numerator = 0, int denominator = 1)
        : n(numerator), d(denominator) {}
    const int numerator() const { return n; }
    const int denominator() const { return d; }

private:
    int n, d;
};

ostream& operator<<(ostream& os, const Rational& r) {
    return os << r.numerator() << " / " << r.denominator() << endl;
}

// Effective-C++ item24: 若所有参数都是都需隐式类型转换, 为此采用non-member-func
const Rational operator*(const Rational& lhs, const Rational& rhs) {
    // 调用构造函数, 传回对象的拷贝(这里由编译器作返回值优化)
    return Rational(lhs.numerator() * rhs.numerator(),
                    lhs.denominator() * rhs.denominator());
}

/* const Rational& operator*(const Rational& lhs, const Rational& rhs) { */
/*     auto tmp = Rational(lhs.numerator() * rhs.numerator(), */
/*                         lhs.denominator() * rhs.denominator()); */
/*     return tmp; */
/* } */
// 若采用引用返回值
// warning: reference to stack memory associated with local variable 'tmp'
// returned [-Wreturn-stack-ad

void t1() {
    //
    Rational a = 10, b(1, 2);
    // 正好相当于调用了c的构造函数
    Rational c = a * b;
    cout << c;
}

int main(int argc, char* argv[]) {
    t1();
    return 0;
}
