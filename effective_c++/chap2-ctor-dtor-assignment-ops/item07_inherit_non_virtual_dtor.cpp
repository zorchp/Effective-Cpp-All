#include <iostream>
using namespace std;
/*
1. 声明基类析构函数为虚函数, 可以让那个派生类的实现定制化,
避免派生类调用基类的析构函数造成派生类资源未被完全释放的情况
2. 如果一个class未含有virtual函数, 通常表示它并不意图被用作一个base class,
此时若声明其析构函数为virtual, 则会造成错误(占用资源变多, 可移植性变差)
3. 经验: 只有当class内含有至少一个virtual函数时, 才为其设置虚析构函数
*/

class NewString : public string {
public:
    NewString(string s) : m_s(s) {}
    string m_s;
};

void t1() {
    NewString* ns = new NewString("123");
    string* ps;
    ps = ns;
    delete ps;
}

class AWOV {
public:
    virtual ~AWOV() = 0;
};

// 一定要实现, 否则最后析构基类的时候会导致链接错误
AWOV::~AWOV() {}

class P : public AWOV {
public:
    void d() {}
    ~P() {}
};

void t2() {
    P p;
    /* p.d(); */
}

int main(int argc, char* argv[]) {
    /* t1(); */
    t2();
    return 0;
}
