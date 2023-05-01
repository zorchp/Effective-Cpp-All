#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <functional>


using namespace std;

const int NUM = 10;

struct P {
    int age;
};

void t1() {
    vector<P *> vp;
    for (int i{}; i < NUM; ++i) vp.emplace_back(new P);
    // mem-leak
}

void t2() {
    // maybe a good way
    vector<P *> vp;
    for (int i{}; i < NUM; ++i) vp.emplace_back(new P);
    // delete by for-loop
    for (auto i = vp.begin(); i != vp.end(); ++i) delete *i;
}

// [clang++] warning: 'unary_function<P *const *, void>' is deprecated
// [-Wdeprecated-declarations]
/*
template <typename T>
struct DelectObject : public unary_function<const T *, void> {
    void operator()(const T ptr) const { delete ptr; }
};
*/

// with c++11 function (variadic template args)
template <typename T>
struct DelectObject : public function<const T *(void)> {
    void operator()(const T ptr) const { delete ptr; }
};

void t3() {
    // good way: by for_each
    vector<P *> vp;
    for (int i{}; i < NUM; ++i) vp.emplace_back(new P);
    // for_each
    for_each(vp.begin(), vp.end(), DelectObject<P *>());
}
class SpecialString : public string {
    // 由于string没有虚析构函数,此时被公有继承会发生问题
    string a;
};

struct DelectObject1 {
    template <typename T>
    void operator()(const T *ptr) const {
        delete ptr;
    }
};

void t4() {
    // special: for string
    deque<SpecialString *> dssp;
    // 不确定的行为, 通过基类指针删除派生的类对象, 而基类又没有虚析构函数
    // error: no matching function for call to object of type
    // 'DelectObject<std::string>'
    // for_each(dssp.begin(), dssp.end(), DelectObject<string>());
    for_each(dssp.begin(), dssp.end(), DelectObject1());
}

void t5() {
    // exception safety version: using shared_ptr
    typedef shared_ptr<P> sp;
    deque<sp> dsp;
    for (int i{}; i < NUM; ++i) dsp.emplace_back(sp(new P));
}


int main(int argc, char const *argv[]) {
    // t1();
    // t2();
    // t3();
    // t4();
    t5();
    return 0;
}