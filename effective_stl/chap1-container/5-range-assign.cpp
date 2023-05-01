#include <iostream>
#include <vector>
using namespace std;

template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v) {
    for (T i : v) os << i << " ";
    return os << endl;
}

vector<int> v1{}, v2{1, 2, 3, 4, 5};

void t1() {
    // 最好的方法
    v1.assign(v2.begin() + v2.size() / 2, v2.end());
}

void t2() {
    for (vector<int>::const_iterator ci = v2.begin() + v2.size() / 2;
         ci != v2.end(); ++ci)
        v1.emplace_back(*ci);
}

void t3() {
    // 使用STL算法, 事实上copy算法内部还是用到了循环
    copy(v2.begin() + v2.size() / 2, v2.end(), back_inserter(v1));
}

// 几乎所有通过插入迭代器限定目标区间的copy调用,
// 都可以转换为利用区间成员函数的调用. 如下:
void t4() {
    // 区间插入
    v1.insert(v1.end(), v2.begin() + v2.size() / 2, v2.end());
}


int main(int argc, char const *argv[]) {
    cout << "v1: " << v1;
    cout << "v2: " << v2;
    cout << "after assign: \n";
    t1();
    // t2();
    // t3();
    // t4();
    cout << "v1: " << v1;
    cout << "v2: " << v2;
    /*
v1:
v2: 1 2 3 4 5
after assign:
v1: 3 4 5
v2: 1 2 3 4 5
*/
    return 0;
}