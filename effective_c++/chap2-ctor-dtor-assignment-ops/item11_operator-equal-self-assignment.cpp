#include <iostream>
using namespace std;
struct Q {};


class P {
    Q* pb;

public:
    P() : pb(new Q) {}
    // 并不是异常安全的
    /* P& operator=(const P& rhs) { */
    /*     // 处理自赋值的逻辑 */
    /*     if (this == &rhs) { */
    /*         cout << "self-assignment\n"; */
    /*         return *this; */
    /*     } */
    /*     delete pb; */
    /*     // 此处如果抛出异常(可能由于内存不足或P的拷贝构造抛出异常), */
    /*     // pb会指向一块被释放了的内存 */
    /*     pb = new Q(*rhs.pb); */
    /*     return *this; */
    /* } */

    // 异常安全, 且满足自赋值的拷贝赋值函数
    /* P& operator=(const P& rhs) { */
    /*     // 先创建临时对象 */
    /*     Q* pq = pb; */
    /*     // 复制pb所指对象之前先不删除pb */
    /*     pb = new Q(*rhs.pb); // 此时就算抛出异常, 原对象仍然保持不变 */
    /*     // 并且自我赋值不受影响(不改变) */
    /*     delete pq; // 最后释放 */
    /*     return *this; */
    /* } */

    // 代码可读性降低, 使用了copy-and-swap操作
    void swap(const P&);
    /* P& operator=(const P& rhs) { */
    /*     P tmp(rhs); */
    /*     swap(tmp); */
    /*     return *this; */
    /* } */

    // 或者, 更进一步:
    //  拷贝赋值操作符可以被声明为值传递方式接受实参
    //  传值方式传递参数会导致拷贝(复制)
    P& operator=(const P& rhs) {
        swap(rhs);
        return *this;
    }
};

void P::swap(const P& rhs) {
    /* using std::swap; */
    /* swap(*rhs.pb, *pb); */
    ::swap(*rhs.pb, *pb);
}

void t1() {
    P p1, p2;
    /* p1 = p1; // self-assignment */
    P *pp1 = new P, *pp2 = pp1;
    pp2 = pp1;
}

int main(int argc, char* argv[]) {
    t1();
    return 0;
}
