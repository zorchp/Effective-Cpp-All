#include <iostream>
using namespace std;

class Empty {};
// equal to:
/*
class Empty {
public:
    Empty() {}
    Empty(const Empty& rhs) {}
    Empty& operator=(const Empty& rhs) {}
    ~Empty() {}
};
*/

template <typename T>
class NamedObj1 {
public:
    NamedObj1(const char* name, const T& val) {}
    NamedObj1(const string& name, const T& val) {}

private:
    string nameVal;
    T objVal;
};

void t1() {
    NamedObj1<int> no1("abc", 2);
    NamedObj1<int> no2(no1);
}

template <typename T>
class NamedObj2 {
public:
    NamedObj2(string& name, const T& val) : nameVal(name), objVal(val) {}

    // copy assign
    NamedObj2<T>& operator=(const NamedObj2& no) {
        this->nameVal = no.nameVal;
        /* this->objVal = no.objVal; */
        // 常量成员不可赋值
        //  Cannot assign to non-static data member 'objVal' with
        //  const-qualified type 'const int'
        return *this;
    }
    // copy ctor
    NamedObj2<T>(const NamedObj2& no)
        : nameVal(no.nameVal), objVal(no.objVal) {}

private:
    string& nameVal;
    const T objVal;
};


void t2() {
    string newDog("p");
    string oldDog("S");
    NamedObj2<int> p(newDog, 2);
    NamedObj2<int> s(oldDog, 9);
    NamedObj2<int> r(p);
    p = s;
}
int main(int argc, char* argv[]) {
    /* t1(); */
    t2();
    return 0;
}
