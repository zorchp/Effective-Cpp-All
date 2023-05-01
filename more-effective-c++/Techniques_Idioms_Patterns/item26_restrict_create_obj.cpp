#include <cstddef>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

using namespace std;

// 不产生对象
class CantBeInstantiated {
private: // 私有化ctor和copy-ctor
    CantBeInstantiated();
    CantBeInstantiated(const CantBeInstantiated&);
};

void t1() {
    /* CantBeInstantiated c; */
    // Calling a private constructor of class 'CantBeInstantiated'
}

class PrintJob {
public:
    PrintJob(const string& whatToPrint) : what(whatToPrint) {}

private:
    string what;
};

class Printer {
public:
    // 要点2: 全局函数作友元, 导致此全局函数不受private 构造函数的限制
    friend Printer& thePrinter();
    void submitJob(const PrintJob& job) { cout << "submitJob" << endl; }
    void reset() { cout << "reset\n"; }
    void performSelfTest() { cout << "performSelfTest\n"; }

private:
    // 要点1: 构造函数访问控制级别为private
    Printer() { cout << "Printer ctor\n"; }
    Printer(const Printer& rhs);
};

// 这种实现用到了友元, 不好的设计. 事实上可以直接通过静态成员函数完成
Printer& thePrinter() {
    // 要点3: 全局函数内含一个static对象, 即只有一个Printer对象会被产生出来
    static Printer p; // 唯一一个打印机对象
    return p;
}

void t2() {
    // 使用方法
    string buf;
    thePrinter().reset();
    thePrinter().submitJob(buf);

    /* Printer ctor */
    /* reset */
    /* submitJob */
}


class Printer1 {
public:
    // 采用静态成员函数的实现
    static Printer1& thePrinter1();
    void submitJob(const PrintJob& job) { cout << "submitJob" << endl; }
    void reset() { cout << "reset\n"; }
    void performSelfTest() { cout << "performSelfTest\n"; }

private:
    // 要点1: 构造函数访问控制级别为private
    Printer1() { cout << "Printer1 ctor\n"; }
    Printer1(const Printer1& rhs);
};

// 这里需要注意, 采用了函数内的static对象, 而不是类内的静态对象,
// 因为类内的静态对象即使未被使用, 也会被初始化
// 而函数内的静态对象如果没有调用行为, 就不会被初始化

// 从初始化时机角度来看, 静态函数的初始化时机就是该函数第一次被调用时,
// 并且在static对象被定义处
// 但是类内的静态对象不一定什么时候初始化: 由于作用域(编译单元)的不同,
// 导致静态成员初始化顺序的不同

// 另一方面, 该函数不能被声明为inline, 即使只有两行, 因为static只能初始化一次,
// 也就是说被调用一次, 而inline的作用是让编译器将每一个调用动作以函数本身替代,
// 但对非成员函数, 意味着这个函数有内部链接, 内部链接会导致该函数在程序中被复制
// 导致函数内的静态对象会被复制多个副本

Printer1& Printer1::thePrinter1() {
    // 要点2: 全局函数内含一个static对象, 即只有一个Printer对象会被产生出来
    static Printer1 p; // 唯一一个打印机对象
    return p;
}


void t3() {
    string buf;
    // 调用方式不一样了
    Printer1::thePrinter1().reset();
    Printer1::thePrinter1().submitJob(buf);
    /* Printer1 ctor */
    /* reset */
    /* submitJob */
}


class Printer2 {
public:
    class TooManyObjs {};

    Printer2();
    ~Printer2();

private:
    static size_t numObjects;
    // 因为打印机个数一直为1, 就不能允许复制行为(实现为private)
    Printer2(const Printer2& rhs);
};

size_t Printer2::numObjects = 0;
Printer2::Printer2() {
    if (numObjects >= 1) throw TooManyObjs();
    ++numObjects;
}

Printer2::~Printer2() { --numObjects; }

void t4() {
    Printer2 p1; //
    Printer2 p2; // libc++abi: terminating with uncaught exception of type
                 // Printer2::TooManyObjs
}

class ColorPrinter : public Printer2 {
public:
    //
};

void t5() {
    Printer2 p1;
    ColorPrinter p2;
    // 此时由于具体类导致了构造计数器认为有两个类
    // 一个基类, 一个派生类的基类部分
    // libc++abi: terminating with uncaught exception of
    // type Printer2::TooManyObjs
}

class Other {};
class CPFMachine {
private:
    Printer2 p;
    Other o;
};

void t6() {
    Printer2 p1;
    CPFMachine p2;
    // 包含亦然
    // libc++abi: terminating with uncaught exception of
    // type Printer2::TooManyObjs
}

// 下面这个类(有限状态自动机, Finite Statement Automaton), 解决了上述问题,
// 通过指针方式确保了对象创建时候的唯一性
class FSA {
public:
    static FSA* makeFSA();
    static FSA* makeFSA(const FSA& rhs);

private:
    FSA() {}
    FSA(const FSA&) {}
};

FSA* FSA::makeFSA() { return new FSA(); }

FSA* FSA::makeFSA(const FSA& rhs) { return new FSA(rhs); }

void t7() {
    unique_ptr<FSA> pfsa1(FSA::makeFSA());
    unique_ptr<FSA> pfsa2(FSA::makeFSA(*pfsa1));
}

class Printer3 {
public:
    static Printer3* makePrinter3();
    static Printer3* makePrinter3(const Printer3&);
    ~Printer3() {}
    class TooManyObjs {};


    void submitJob(const PrintJob&);
    void reset() { cout << "reset\n"; }
    void performSelfTest() { cout << "performSelfTest\n"; }

private:
    static size_t numObjects;
    static const size_t maxObjects = 10; // 最多创建的对象个数
    Printer3();
    Printer3(const Printer3&);
};
size_t Printer3::numObjects = 0;
const size_t Printer3::maxObjects; // 义务性定义

Printer3::Printer3() {
    if (numObjects >= maxObjects) throw TooManyObjs();
    ++numObjects;
}
Printer3::Printer3(const Printer3& rhs) {
    if (numObjects >= maxObjects) throw TooManyObjs();
    ++numObjects;
}

Printer3* Printer3::makePrinter3() { return new Printer3; }
Printer3* Printer3::makePrinter3(const Printer3& rhs) {
    return new Printer3(rhs);
}

void t8() {
    /* Printer3 p1; */
    Printer3* p2 = Printer3::makePrinter3(); // ok, 间接调用default-ctor
    /* Printer3 p3 = *p2; */

    p2->performSelfTest();
    p2->reset();

    /* performSelfTest */
    /* reset */
    delete p2;
}

// 创建对象计数模板类
template <class BeingCounted>
class Counted {
public:
    class TooManyObjs {};
    static int objectCount() { return numObjs; }

protected:
    // 只被用来作为基类
    Counted();
    Counted(const Counted&);
    ~Counted() { --numObjs; }

private:
    static size_t numObjs;
    static const size_t maxObjects;
    void init(); // 避免出现重复代码
};

template <class BeingCounted>
Counted<BeingCounted>::Counted() {
    init();
}

template <class BeingCounted>
Counted<BeingCounted>::Counted(const Counted<BeingCounted>&) {
    init();
}

template <class BeingCounted>
size_t Counted<BeingCounted>::numObjs = 0;


// 如果不加, 并且类内未定义, 就会报链接错误
template <class BeingCounted>
const size_t Counted<BeingCounted>::maxObjects = 10;

template <class BeingCounted>
void Counted<BeingCounted>::init() {
    if (numObjs >= maxObjects) throw TooManyObjs();
    ++numObjs;
}

// 使用上面创建的模板类完成 Printer 创建
// 终极版Printer: 计数不应对外暴露, 设置为私有继承
class Printer_Counter : private Counted<Printer_Counter> {
public:
    static Printer_Counter* makePrinter();
    static Printer_Counter* makePrinter(const Printer_Counter& rhs);
    ~Printer_Counter() {}

    void submitJob(const PrintJob&);
    void reset() { cout << "reset\n"; }
    void performSelfTest() { cout << "performSelfTest\n"; }

    // 重点:
    // 让下面两个函数对Printer_Counter的用户而言为public的
    using Counted<Printer_Counter>::objectCount;
    using Counted<Printer_Counter>::TooManyObjs;

private:
    Printer_Counter() { cout << "call Printer_Counter()\n"; }
    Printer_Counter(const Printer_Counter&) {
        cout << "call Printer_Counter(const Printer_Counter&)\n";
    }
};

Printer_Counter* Printer_Counter::makePrinter() { return new Printer_Counter; }
Printer_Counter* Printer_Counter::makePrinter(const Printer_Counter& rhs) {
    return new Printer_Counter(rhs);
}

void t9() {
    //
    Printer_Counter* p1 = Printer_Counter::makePrinter();
    p1->reset();
    p1->performSelfTest();
    cout << p1->objectCount() << endl;
    delete p1;

    /* call Printer_Counter() */
    /* reset */
    /* performSelfTest */
    /* 1 */
}


int main(int argc, char* argv[]) {
    /* t2(); */
    /* t3(); */
    /* t4(); */
    /* t5(); */
    /* t6(); */
    /* t7(); */
    /* t8(); */
    t9();
    return 0;
}
