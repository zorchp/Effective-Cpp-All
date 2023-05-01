#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

struct Widget {
    Widget(int ii) : num(ii) {}

    int num;
};


class BadPredicate {
public:
    BadPredicate() : timesCalled(0) {}
    bool operator()(const Widget &) { return ++timesCalled == 3; }

private:
    size_t timesCalled;
};

void t1() {
    vector<Widget> vw;
    for (int i{1}; i <= 10; ++i) vw.emplace_back(Widget(i));
    for (auto item : vw) cout << item.num << " ";
    cout << endl;
    vw.erase(remove_if(vw.begin(), vw.end(), BadPredicate()), vw.end());
    for (auto item : vw) cout << item.num << " ";
    // 多删除了一个元素,
    // 由于传参时remove_if内部调用remove_copy_if复制了一份predicate对象
    // 1 2 3 4 5 6 7 8 9 10
    // 1 2 4 5 7 8 9 10
}


class BadPredicate1 {
public:
    BadPredicate1() {}
    bool operator()(const Widget &) {
        static size_t ii = 0; // 这样可以, 但是不好
        return ++ii == 3;
    }
};

void t2() {
    vector<Widget> vw;
    for (int i{1}; i <= 10; ++i) vw.emplace_back(Widget(i));
    for (auto item : vw) cout << item.num << " ";
    cout << endl;
    vw.erase(remove_if(vw.begin(), vw.end(), BadPredicate1()), vw.end());
    for (auto item : vw) cout << item.num << " ";
    // 1 2 3 4 5 6 7 8 9 10
    // 1 2 4 5 6 7 8 9 10
}


int main(int argc, char const *argv[]) {
    // t1();
    t2();
    return 0;
}