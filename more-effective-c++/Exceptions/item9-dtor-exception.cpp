#include <iostream>
using namespace std;

class ALA {
public:
    ALA() {}
    virtual void process() = 0;
    virtual ~ALA() { cout << "~ALA\n"; }
};

class Puppy : public ALA {
public:
    Puppy() {}
    virtual void process() { cout << "call Puppy::process()\n"; }
};

void process1() {
    ALA* pa = new Puppy();
    pa->process();
    // throw;
    delete pa;
}

void process2() {
    ALA* pa = new Puppy();
    try {
        // throw 1;
        pa->process(); // 就算出现异常, pa也会正常析构, 但是导致代码可读性降低
    } catch (...) {
        delete pa;
        cout << "pa deleted by catch\n";
        throw;
    }
    cout << "pa deleted normally...\n";
    delete pa;
}

void process3() {
    // 采用智能指针
    unique_ptr<ALA> pa(new Puppy());
    pa->process();
    // 可以不用delete, 直接由智能指针管理内存释放
    // call Puppy::process()
    // ~ALA
}

void t1() {
    // process1(); // 异常导致mem-leak
    // process2();
    process3();
}

int main(int argc, char const* argv[]) {
    t1();
    return 0;
}
