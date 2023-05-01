#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <functional>


using namespace std;

struct P {
    P(int i) : age(i) {}
    void get() { cout << age << " "; }
    int age;
};

void t1() {
    list<P> lp{1, 2, 3};
    // 直接循环, 每一次都会与end()比较
    for (auto i = lp.begin(); i != lp.end(); ++i) (*i).get(); // 1 2 3
    cout << endl;
    // use algorithm: 仅调用一次end()
    // for_each(lp.begin(), lp.end(), mem_fun_ref(&P::get)); // 1 2 3
    for_each(lp.begin(), lp.end(), mem_fn(&P::get)); // 1 2 3
}

size_t fillArray(double* pArr, size_t arraySize) {
    // 假设数组足够大
    int NUM = arraySize;
    for (int i{}; i < NUM; ++i) pArr[i] = i;
    return NUM;
}

void t2() {
    const int maxNum = 10;
    double data[maxNum];
    deque<double> d;
    size_t numDoubles = fillArray(data, maxNum);
    for (auto i : data) cout << i << " ";
    cout << endl;
    for (size_t i{}; i < numDoubles; ++i) d.insert(d.begin(), data[i] + 41);
    for (auto i : d) cout << i << " ";
    cout << endl;
    // 反序了
    // 0 1 2 3 4 5 6 7 8 9
    // 50 49 48 47 46 45 44 43 42 41
}

void t3() {
    const int maxNum = 10;
    double data[maxNum];
    deque<double> d;
    size_t numDoubles = fillArray(data, maxNum);
    for (auto i : data) cout << i << " ";
    cout << endl;
    auto it = d.begin();
    for (size_t i{}; i < numDoubles; ++i) {
        it = d.insert(it, data[i] + 41);
        ++it;
    }
    for (auto i : d) cout << i << " ";
    cout << endl;
    // 0 1 2 3 4 5 6 7 8 9
    // 41 42 43 44 45 46 47 48 49 50
}

void t4() {
    const int maxNum = 10;
    double data[maxNum];
    deque<double> d;
    size_t numDoubles = fillArray(data, maxNum);
    for (auto i : data) cout << i << " ";
    cout << endl;

    // 使用算法: (或者`back_inserter(d)`)
    transform(data, data + numDoubles, inserter(d, d.begin()),
              [](double x) { return x + 41; });

    for (auto i : d) cout << i << " ";
    cout << endl;
    // 0 1 2 3 4 5 6 7 8 9
    // 41 42 43 44 45 46 47 48 49 50
}

static vector<int> v{1, 2, 4, 7, 10, 6, 8};
static int x{4}, y{8};

void t5() {
    vector<int>::iterator i = v.begin();
    for (; i != v.end(); ++i)
        if (*i > x && *i < y) break;
    cout << *i << endl; // 7
}
using namespace std::placeholders;
void t6() {
    auto i = find_if(v.begin(), v.end(),
                     bind(logical_and<bool>(), bind(greater<int>(), _1, x),
                          bind(less<int>(), _1, y)));
    cout << *i << endl; // 7
}

// 写一个函数子类
template <typename T>
class BetweenValues {
public:
    BetweenValues(const T& low, const T& high) : lowVal(low), highVal(high) {}
    bool operator()(const T& val) const {
        return val > lowVal && val < highVal;
    }

private:
    T lowVal, highVal;
};

void t7() {
    auto i = find_if(v.begin(), v.end(), BetweenValues<int>(x, y));
    cout << *i << endl; // 7
}

void t8() {
    // best way
    auto i = find_if(v.begin(), v.end(),
                     [&](int val) { return val > x && val < y; });
    cout << *i << endl; // 7
}

int main(int argc, char const* argv[]) {
    // t1();
    // t2();
    // t3();
    // t4();
    // t5();
    // t6();
    // t7();
    t8();
    return 0;
}