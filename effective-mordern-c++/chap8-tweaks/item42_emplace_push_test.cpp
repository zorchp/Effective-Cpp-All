#include <vector>
#include <iostream>
using namespace std;


class P {
    [[maybe_unused]] int x;

public:
    P() { cout << __PRETTY_FUNCTION__ << endl; }
    P(int) { cout << __PRETTY_FUNCTION__ << endl; }
    ~P() { cout << __PRETTY_FUNCTION__ << endl; }
    P(const P&) { cout << __PRETTY_FUNCTION__ << endl; }
    P& operator=(const P&) {
        cout << __PRETTY_FUNCTION__ << endl;
        return *this;
    }
    P(P&&) { cout << __PRETTY_FUNCTION__ << endl; }
    P& operator=(P&&) {
        cout << __PRETTY_FUNCTION__ << endl;
        return *this;
    }
};


void t1() {
    // push_back:
    vector<P> vp;
    cout << string(60, '=') << endl;

    cout << string(60, '=') << endl;
    vp.push_back(12);
    // vp.emplace_back(12); // just P::P(int)
    cout << string(60, '=') << endl;
    // P p;
    // vp.push_back(p);// 扩容机制导致两次拷贝
    // vp.emplace_back(p);
    // P::P(const P&)
    // ============================================================
    // P::P(int)
    // P::P(P&&)
    // P::~P()
    // ============================================================
}

void t2() {
    //
}

int main(int argc, char* argv[]) {
    t1();
    return 0;
}
