#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>


using namespace std;

void t1() {
    vector<int> v{1, 2, 3, 6, 4, 2, 3};
    int x{4}, y{5};
    for (auto i : v) cout << i << " ";
    cout << endl;
    // 删除最后一个大于等于y的元素之后的所有小于x的元素
    v.erase(
        remove_if(
            find_if(v.rbegin(), v.rend(), [&](int v) { return v >= y; }).base(),
            v.end(), [&](int v) { return v < x; }),
        v.end());
    for (auto i : v) cout << i << " ";
    // 1 2 3 6 4 2 3
    // 1 2 3 6 4
}

int main(int argc, char const *argv[]) {
    t1();
    return 0;
}
