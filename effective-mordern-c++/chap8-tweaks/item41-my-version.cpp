#include <format>
#include <iostream>
#include <vector>
using namespace std;
// effective modern c++ item 41

class P {
    [[maybe_unused]] int x;

public:
    P() { cout << __PRETTY_FUNCTION__ << endl; }
    ~P() { cout << __PRETTY_FUNCTION__ << endl; }
    P(const P &) { cout << __PRETTY_FUNCTION__ << endl; }
    P &operator=(const P &) {
        cout << __PRETTY_FUNCTION__ << endl;
        return *this;
    }
    P(P &&) { cout << __PRETTY_FUNCTION__ << endl; }
    P &operator=(P &&) {
        cout << __PRETTY_FUNCTION__ << endl;
        return *this;
    }
};

class DataHolder {
    P m_data;

public:
    // void set(const P& data) { m_data = data; }       // version1
    // void set(P&& data) { m_data = std::move(data); } // version2
    template <typename T>
    void set(T &&data) {
        m_data = std::forward<T>(data);
    } // version3
      // void set(P data) { m_data = std::move(data); } // version4: best
};

const string delimiter = string(30, '=');

void t1() {
    DataHolder wrapper;
    P p;
    cout << format("{} case {} {}\n", delimiter, 1, delimiter);
    wrapper.set(p);
    cout << format("{} case {} {}\n", delimiter, 2, delimiter);
    wrapper.set(P{});
    cout << format("{} end case {}\n", delimiter, delimiter);
    // only version 1
    // ============================== case 1 ==============================
    // P& P::operator=(const P&) 对具名对象执行拷贝赋值
    // ============================== case 2 ==============================
    // P::P()
    // P& P::operator=(const P&) 这里执行 了对临时对象的拷贝
    // P::~P()
    // ============================== end case ==============================

    // version1 and version2 需要重载函数, 不方便
    // ============================== case 1 ==============================
    // P& P::operator=(const P&)
    // ============================== case 2 ==============================
    // P::P()
    // P& P::operator=(P&&)
    // P::~P()
    // ============================== end case ==============================

    // version3 需要用模版展开, 代码膨胀
    // ============================== case 1 ==============================
    // P& P::operator=(const P&)
    // ============================== case 2 ==============================
    // P::P()
    // P& P::operator=(P&&)
    // P::~P()
    // ============================== end case ==============================

    // only version4, best version, 针对左值 Copy, 右值 move
    // ============================== case 1 ==============================
    // P::P(const P&)
    // P& P::operator=(P&&)
    // P::~P()
    // ============================== case 2 ==============================
    // P::P()
    // P& P::operator=(P&&)
    // P::~P()
    // ============================== end case ==============================
}

int main(int argc, char *argv[]) {
    t1();
    return 0;
}
