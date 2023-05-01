#include <iostream>
#include <ostream>
#include <string>
#include <vector>
using namespace std;

class UPInt {
public:
    UPInt() : a(0) {}
    UPInt(int value) : a(value) {}
    const int val() const { return a; }

private:
    int a;
};

// 重载消除隐式类型转换, 但是事实上这是没必要的,
// 产生隐式类型转换不会很大程度上降低程序运行效率
// 这里提供了一种用重载消除影响的方法
const UPInt operator+(const UPInt &lhs, const UPInt &rhs) {
    return lhs.val() + rhs.val();
}
const UPInt operator+(const UPInt &lhs, const int &rhs) {
    return lhs.val() + rhs;
}
const UPInt operator+(const int &lhs, const UPInt &rhs) {
    return lhs + rhs.val();
}
// 错误, 内置类型不应该重载
/* const UPInt operator+(const int &lhs, const int &rhs) { return lhs + rhs; }
 */

ostream &operator<<(ostream &os, const UPInt &u) {
    return os << u.val() << endl;
}

void t1() {
    UPInt up1, up2;
    UPInt up3 = up1 + up2;
    cout << up3;
}

int main(int argc, char *argv[]) {
    t1();
    return 0;
}
