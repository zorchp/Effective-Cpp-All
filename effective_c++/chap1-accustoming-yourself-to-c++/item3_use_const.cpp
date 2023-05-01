#include <iostream>
#include <string>

using namespace std;


class TextBlock {
public:
    TextBlock(const string& s) : text(s) {}
    string get() { return text; }
    void* getaddr() { return &text; }
    const string get() const { return text; }
    // 代码重复
    const char& operator[](size_t pos) const { return text[pos]; }
    char& operator[](size_t pos) { return text[pos]; }

private:
    string text;
};


void t1() {
    TextBlock tb("hello");
    cout << tb.get() << endl;
    cout << tb.getaddr() << endl; // 地址不变
    tb[0] = 'o';                  // operator[]函数返回引用
    cout << tb.get() << endl;
    cout << tb.getaddr() << endl; // 地址不变
}

void t2() {
    const TextBlock ctb("world");
    cout << ctb.get() << endl;
    // ctb[0] = 'e';//error
    cout << ctb.get() << endl;
}

class TextBlock1 {
public:
    TextBlock1(const string& s) : text(s) {}
    string get() { return text; }
    void* getaddr() { return &text; }
    const string get() const { return text; }
    const char& operator[](size_t pos) const { return text[pos]; }
    char& operator[](size_t pos) {
        return const_cast<char&>(static_cast<const TextBlock1&>(*this)[pos]);
    }
    // 但是不要在const成员函数中调用non-const成员实现代码简化

private:
    string text;
};

void t3() {
    //
    TextBlock tb("hello");
    cout << tb.get() << endl;
    cout << tb.getaddr() << endl; // 地址不变
    tb[0] = 'o';                  // operator[]函数返回引用
    cout << tb.get() << endl;
    cout << tb.getaddr() << endl; // 地址不变
    // hello
    // 0x16ee96de8
    // oello
    // 0x16ee96de8
}

class Rational {
public:
    Rational(int numerator = 0, int denominator = 1)
        : n(numerator), d(denominator) {}
    int numerator() const { return n; }
    int denominator() const { return d; }

private:
    int n, d;
};

ostream& operator<<(ostream& os, const Rational& r) {
    return os << r.numerator() << " / " << r.denominator() << endl;
}


// 若所有参数都是都需隐式类型转换, 为此采用non-member-func
// const Rational operator*(const Rational& lhs, const Rational& rhs) {
// 如果不加const, 会发生什么?
Rational operator*(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.numerator() * rhs.numerator(),
                    lhs.denominator() * rhs.denominator());
}


void t4() {
    //
    Rational a(1, 2), b = a, c;
    (a * b) = c; // 可通过编译!!
    cout << c;
}


int main(int argc, char const* argv[]) {
    // t1();
    // t2();
    // t3();
    t4();
    return 0;
}