#include <algorithm>
#include <cstddef>
#include <iostream>
#include <list>
#include <memory>
#include <new>

using namespace std;

// 一个只能诞生于heap内存中的数值类
class UPNumber {
public:
    UPNumber() {}
    UPNumber(int initval);
    UPNumber(double initval);
    UPNumber(const UPNumber&);

    void destory() const { delete this; }

    /* private: */
protected:
    // 这里是重点
    ~UPNumber() {}
};

void t1() {
    /* UPNumber n; // error, 会隐式调用dtor */
    UPNumber* p = new UPNumber;

    /* delete p; */
    p->destory();
}

// 但是这会阻止继承和组合(内含)
class NonNegativeUPNumber : public UPNumber {};

class Asset {
private:
    UPNumber val;
};

void t2() {
    /* NonNegativeUPNumber n1; */
    /* Asset a1; */
}

// 把基类的dtor访问类型变成protected
// 派生类可以访问保护成员
class NonNegativeUPNumber1 : public UPNumber {};

class Asset1 {
private:
    // 变成委托(delegation)
    UPNumber* val;
};

void t3() {
    // ok
    NonNegativeUPNumber1 n1;
    Asset1 a1;
}

// 判断某对象是不是在heap内存中

class UPNumber1 {
public:
    class HeapConstraintViolation {};
    static void* operator new(size_t size);
    UPNumber1();

private:
    static bool onTheHeap;
};

bool UPNumber1::onTheHeap = false;

void* UPNumber1::operator new(size_t size) {
    onTheHeap = true;
    return ::operator new(size);
}

UPNumber1::UPNumber1() {
    if (!onTheHeap) throw HeapConstraintViolation();
    onTheHeap = false;
}

void t4() {
    // 但是这个是个特例, operator new调用一次, 分配内存之后, 调用100次ctor,
    // 而第二次会变为false从而throw
    /* UPNumber1* numArray = new UPNumber1[100]; */

    UPNumber1* pn = new UPNumber1(*new UPNumber1()); // 忽略mem-leak
    // 这段代码涉及到一个函数调用顺序的问题
}

// 失败的尝试, 因为不同系统的实现不同, 此外还要考虑static类型
bool onHeap(const void* addr) {
    char OnTheStack;
    return addr < &OnTheStack;
}

void allocateSomeObjs() {
    char* pc = new char;
    cout << onHeap(pc) << endl; // 1
    char c;
    cout << onHeap(&c) << endl;  // 0
    static char sc;              // 事实上存在于static区
    cout << onHeap(&sc) << endl; // 1
    // clang++15: 101, g++-12: 001
}


typedef const void* RawAddress;

class HeapTracked {
public:
    class MissingAddress {};
    virtual ~HeapTracked() = 0;
    static void* operator new(size_t size);
    static void operator delete(void* ptr);
    bool isOnHeap() const;

private:
    static list<RawAddress> addresses;
};

list<RawAddress> HeapTracked::addresses;
HeapTracked::~HeapTracked() {}

void* HeapTracked::operator new(size_t size) {
    void* memPtr = ::operator new(size);
    addresses.push_front(memPtr);
    return memPtr;
}

void HeapTracked::operator delete(void* ptr) {
    list<RawAddress>::iterator it =
        find(addresses.begin(), addresses.end(), ptr);
    if (it != addresses.end()) {
        addresses.erase(it);
        ::operator delete(ptr);
    } else {
        //'operator delete' has a non-throwing exception specification but can
        // still throw
        throw MissingAddress();
    }
}

bool HeapTracked::isOnHeap() const {
    // 下面这行
    RawAddress rawaddress = dynamic_cast<RawAddress>(this);

    list<RawAddress>::iterator it =
        find(addresses.begin(), addresses.end(), rawaddress);
    return it != addresses.end();
}

class Asset2 : public HeapTracked {
private:
    /* UPNumber1 val; */
    UPNumber1* ptr = new UPNumber1;
};

void inventoryAsset(const Asset2* ap) {
    if (ap->isOnHeap())
        cout << "heap-based\n";
    else
        cout << "stack-based\n";
}

void t5() {
    Asset2* ap = new Asset2;
    inventoryAsset(ap); // heap-based

    Asset2 ap1;
    inventoryAsset(&ap1); // stack-based
}

// 禁止对象产生于heap中
class NoHeap_UPNumber {
private:
    static void* operator new(size_t size);
    static void operator delete(void* ptr);
};

void t6() {
    NoHeap_UPNumber n1; // ok
    static NoHeap_UPNumber n2;
    /* auto i = new NoHeap_UPNumber(); // error */
}

// 但是基类继承会有一些问题
class NonNegativeUPNumber2 : public NoHeap_UPNumber {};

void t7() {
    NonNegativeUPNumber2 n1;
    static NonNegativeUPNumber2 n2;
    /* auto i = new NonNegativeUPNumber2(); // error */
}

class Asset3 {
public:
    Asset3(int initval) {}

private:
    // 内含, 没有问题
    NoHeap_UPNumber value;
};

void t8() {
    Asset3* pa = new Asset3(100);
    // ok, 调用的是Asset3::operator new, 或者::operator new,
    // 不是NoHeap_UPNumber::operator new
}

int main(int argc, char* argv[]) {
    /* t1(); */
    /* t2(); */
    /* t3(); */
    /* t4(); */
    /* allocateSomeObjs(); */
    /* t5(); */
    t8();
    return 0;
}
