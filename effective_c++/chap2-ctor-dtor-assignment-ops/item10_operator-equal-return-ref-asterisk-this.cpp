#include <iostream>

using namespace std;


void t1() {
    // 赋值采用右结合律:
    int x, y, z;
    x = y = z = 10;
    cout << x << " " << y << " " << z << endl; // 10 10 10
    x = (y = (z = 15));
    cout << x << " " << y << " " << z << endl; // 15 15 15
}

class P {
public:
    // 注意, 这里返回的是引用, 为了满足上面的链式调用
    P& operator=(const P& rhs) { return *this; }
    P& operator+=(const P& rhs) { return *this; }
    P& operator=(int rhs) { return *this; }
};

int main(int argc, char* argv[]) {
    t1();
    return 0;
}
