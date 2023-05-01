#include <iostream>
#include <iterator>
#include <ostream>
#include <ratio>
#include <vector>

using namespace std;
template <typename T>
class Rational;

template <typename T>
const Rational<T> doMultiply(const Rational<T>&, const Rational<T>&);

template <typename T>
class Rational {
public:
    // 如果不这样, 会导致link error
    friend const Rational operator*(const Rational& lhs, const Rational& rhs) {
        /* return Rational<T>(lhs.numerator() * rhs.numerator(), */
        /*                    lhs.denominator() * rhs.denominator()); */
        return doMultiply(lhs, rhs);
    }

    Rational(const T& numerator = 0, const T& denominator = 1)
        : m_numerator(numerator), m_denominator(denominator) {}
    const T numerator() const { return m_numerator; }
    const T denominator() const { return m_denominator; }

private:
    T m_numerator;
    T m_denominator;
};

template <typename T>
const Rational<T> doMultiply(const Rational<T>& lhs, const Rational<T>& rhs) {
    return Rational<T>(lhs.numerator() * rhs.numerator(),
                       lhs.denominator() * rhs.denominator());
}

template <typename T>
ostream& operator<<(ostream& os, const Rational<T>& r) {
    os << r.numerator() << " / " << r.denominator() << endl;
    return os;
}

void t1() {
    //
    Rational<int> oneHalf(1, 2);
    cout << oneHalf;
    Rational<int> ans = oneHalf * 2;
    cout << ans;
    /* 1 / 2 */
    /* 2 / 2 */
}
int main(int argc, char* argv[]) {
    t1();
    return 0;
}
