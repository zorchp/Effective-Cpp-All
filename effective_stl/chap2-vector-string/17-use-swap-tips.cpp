#include <string>
#include <vector>
#include <iostream>
using namespace std;

void t1() {
    vector<int> v{};
    for (int i{}; i < 1200; ++i) v.push_back(i);
    cout << v.size() << endl;     // 1200
    cout << v.capacity() << endl; // 2048
    cout << "after `vector<int>(v).swap(v)`:\n";
    vector<int>(v).swap(v);
    cout << v.size() << endl;     // 1200
    cout << v.capacity() << endl; // 1200
    cout << "after `vector<int>().swap(v)`:\n";
    vector<int>().swap(v);
    cout << v.size() << endl;     // 0
    cout << v.capacity() << endl; // 0
}

int main(int argc, char const *argv[]) {
    t1();
    return 0;
}