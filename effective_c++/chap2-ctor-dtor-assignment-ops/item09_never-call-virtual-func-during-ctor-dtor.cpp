#include <iostream>
using namespace std;

class Transaction {
public:
    Transaction();
    virtual void logTrans() const = 0;
};

Transaction::Transaction() {
    // 此时的虚函数仍然是基类的版本, 而非多态版本
    // Call to pure virtual member function 'logTrans' has undefined behavior;
    // overrides of 'logTrans' in subclasses are not available in the cons
    /* logTrans(); */

    // 由于base class的构造函数执行早于derived class的构造函数, 当base类
    // 构造函数执行时, derived的成员变量尚未初始化.
    // 如果此时调用的virtual函数下降至derived类,
    // 而derived的函数几乎必然调用本地成员变量, 而那些成员变量还未初始化,
    // 于是造成未定义的行为

    // 在基类构造期间, 虚函数不是虚函数(还未真正完成虚函数的使命: 产生多态行为)
}

class Derived1 : public Transaction {
public:
    virtual void logTrans() const;
};

class Derived2 : public Transaction {
public:
    virtual void logTrans() const;
};

void t1() {
    //
    Derived1 d1;
}

int main(int argc, char* argv[]) {
    t1();
    /* t2(); */
    /* t3(); */
    return 0;
}
