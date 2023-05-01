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
    const Rational operator*(const Rational& rhs);

private:
    int n, d;
};

ostream& operator<<(ostream& os, const Rational& r) {
    return os << r.numerator() << " / " << r.denominator() << endl;
}

const Rational Rational::operator*(const Rational& rhs) {
    return Rational(this->numerator() * rhs.numerator(),
                    this->denominator() * rhs.denominator());
}

// 若所有参数都是都需隐式类型转换, 为此采用non-member-func
const Rational operator*(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.numerator() * rhs.numerator(),
                    lhs.denominator() * rhs.denominator());
}


void t1() {
    //
    Rational a(1, 2);
    Rational b = a * 2;
    Rational c = 2 * a;
    cout << b; // 2 / 2
    cout << c;
}

int main(int argc, char* argv[]) {
    t1();
    return 0;
}
