#include <vector>
#include <iostream>
#include <string>
using namespace std;

class Widget {
public:
    //
};

vector<bool> features(const Widget& w) {
    return vector<bool>{0, 1, 1, 1, 0, 1, 0};
}

void processWidget(const Widget& w, bool isPriority) { cout << "ok\n"; }

void t1() {
    Widget w;
    bool highPriority = features(w)[5];
    processWidget(w, highPriority); // ok
}
void t2() {
    Widget w;
    auto highPriority = features(w)[5];
    cout << typeid(highPriority).name() << endl; // auto返回的对象类型是代理类的
    // g++: St14_Bit_reference
    // NSt3__115__bit_referenceINS_6vectorIbNS_9allocatorIbEEEELb1EEE
    // std::__1::__bit_reference<std::__1::vector<bool,
    // std::__1::allocator<bool> >, true>

    processWidget(w, highPriority); // undefined behaviour
    //==11881==ERROR: AddressSanitizer: heap-use-after-free on address
    // 0x000107b00710 at pc 0x000104db4318 bp 0x00016b04eb90 sp 0x00016b04eb88
}

void t3() {
    Widget w;
    // 避免隐式类型转换, 采用显式
    auto highPriority = static_cast<bool>(features(w)[5]);
    processWidget(w, highPriority); // ok
}

double calcEps() { return 1.0; }

void t4() {
    float ep1 = calcEps(); // 隐式类型转换
    auto ep2 = calcEps();
    double d = 1.8;
    vector<int> v{1, 2, 3};
    //
    int index = d * v.size();
    auto index1 = static_cast<int>(d * v.size());
    cout << index << endl;
    cout << index1 << endl;
}

int main(int argc, char* argv[]) {
    /* t1(); */
    /* t2(); */
    /* t3(); */
    t4();
    return 0;
}
