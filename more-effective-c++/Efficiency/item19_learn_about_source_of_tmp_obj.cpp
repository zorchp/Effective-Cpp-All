#include <iostream>
#include <string>
using namespace std;

// 若为引用, 则会报错(因为编译器禁止修改临时变量, 或者可以说临时变量具有`常性`)
/* size_t countChar(string& str, char ch) { */
size_t countChar(const string& str, char ch) {
    int cnt{};
    for (char c : str)
        if (c == ch) ++cnt;
    return cnt;
}

void t1() {
#define NUM 1000
    char buf[NUM];
    char c;
    cin >> c >> buf;
    // 传参与形参不一致, 产生临时对象(隐式类型转换)
    cout << "countChar: " << countChar(buf, c) << "\n";
}


int main(int argc, char* argv[]) {
    t1();
    return 0;
}
