#include <exception>
#include <iostream>
#include <istream>
#include <list>
#include <ostream>

using namespace std;


class NLComponent {
public:
    // 虚拷贝构造函数
    virtual NLComponent *clone() const = 0;
    // 为了做一个虚化的非成员函数
    //  not a good way
    /* virtual ostream &operator<<(ostream &str) const = 0; */
    virtual ostream &print(ostream &s) const = 0;
};

class TextBlock : public NLComponent {
public:
    // virtual copy-ctor
    virtual TextBlock *clone() const { return new TextBlock(*this); }
    // not a good way
    /* virtual ostream &operator<<(ostream &str) const; */
    virtual ostream &print(ostream &s) const;
};

class Graphic : public NLComponent {
public:
    //
    // virtual copy-ctor
    virtual Graphic *clone() const { return new Graphic(*this); }
    // not a good way
    /* virtual ostream &operator<<(ostream &str) const; */
    virtual ostream &print(ostream &s) const;
};

class NewsLetter {
public: //
    // copy-ctor
    NewsLetter(const NewsLetter &rhs);
    static NLComponent *readComponent(istream &str);
    NewsLetter(istream &str);

private:
    list<NLComponent *> components;
};

// 正常的拷贝构造实现
NewsLetter::NewsLetter(const NewsLetter &rhs) {
    using lci = list<NLComponent *>::const_iterator;
    for (lci it = rhs.components.begin(); it != rhs.components.end(); ++it) {
        // 遍历容器实现
        components.push_back((*it)->clone());
    }
}


NewsLetter::NewsLetter(istream &str) {
    /* while (str) */
    // read something
    /* components.push_back(readComponent(str)); */
}

// 此为虚化的非成员函数
inline ostream &operator<<(ostream &os, const NLComponent &c) {
    return c.print(os);
}


void t1() {
    //
    /* TextBlock t; */
    /* t << cout; */
    /* // 等价于 */
    /* t.operator<<(cout); */
}

int main(int argc, char *argv[]) {
    t1();
    return 0;
}
