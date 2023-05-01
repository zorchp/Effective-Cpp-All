#include <algorithm>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <chrono>
using namespace std;
using namespace chrono;
/* const int SIZE = 100000000; */

class Widget;
typedef map<int, Widget> IntWidgetMap;

class Widget {
    double weight;
    friend ostream& operator<<(ostream& os, Widget& w) {
        return os << w.weight;
    }

public:
    Widget() {}
    Widget(double w) : weight(w) {}
    Widget& operator=(double w) {
        this->weight = w;
        return *this;
    }
};

ostream& operator<<(ostream& os, IntWidgetMap& m) {
    for (auto [k, v] : m) os << k << " : " << v << endl;
    /* for (auto x : m) os << x.first << " : " << x.second << endl; */
    return os;
}

void t1() {
    map<int, Widget> m1;
    m1[1] = 1.50;
    pair<IntWidgetMap::iterator, bool> res =
        m1.insert(IntWidgetMap::value_type(1, Widget()));
    res.first->second = 1.50;
    m1.insert(IntWidgetMap::value_type(2, 2.50));
    cout << m1;
}

void t2() {
    map<int, Widget> m2;
    m2.insert(IntWidgetMap::value_type(2, 2.50));
    m2[2] = 3.5;
    cout << m2;
    m2.insert(IntWidgetMap::value_type(2, 3.6)).first->second = 3.6;
    cout << m2;
}

template <typename MapType, typename KeyArgType, typename ValArgType>
typename MapType::iterator efficientAddOrUpdate(MapType& m, const KeyArgType& k,
                                                const ValArgType& v) {
    typename MapType::iterator lb = m.lower_bound(k); // 不在k之前的键对应的迭代器
    // 如果lb指向的键与k等价, 更新
    if (lb != m.end() && !(m.key_comp()(k, lb->first))) {
        lb->second = v;
        return lb;
    } else { // 插入
        typedef typename MapType::value_type MVT;
        return m.insert(lb, MVT(k, v));
    }
}

void t3() {
    IntWidgetMap m;
    efficientAddOrUpdate(m, 12, 1.3);
    cout << m;
}
/* void time_used(void (*t)(void)) { */
/*     auto start = system_clock::now(); */
/*     t(); */
/*     auto end = system_clock::now(); */
/*     auto duration = duration_cast<microseconds>(end - start); */
/*     cout << "花费了" */
/*          << double(duration.count()) * microseconds::period::num / */
/*                 microseconds::period::den */
/*          << "秒" << endl; */
/* } */
int main(int argc, char* argv[]) {
    /* t1(); */
    /* t2(); */
    t3();
    return 0;
}
