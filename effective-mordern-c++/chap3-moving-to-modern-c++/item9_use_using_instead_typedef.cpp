#include <iostream>
#include <unordered_map>
#include <memory>
#include <list>

using namespace std;


void t1() {
    //
    typedef std::unique_ptr<std::unordered_map<std::string, std::string>>
        UPtrMapSS;
    using UPtrMapSS =
        std::unique_ptr<std::unordered_map<std::string, std::string>>;
}

void t2() {
    typedef void (*FP)(int, const string &);
    using FP = void (*)(int, const string &);
}

template <typename T>
class MyAlloc : public allocator<T> {};

// 针对模板, 只能使用using的别名声明
template <typename T>
using MyAllocList_using = list<T, MyAlloc<T>>;

// 使用typedef也不是不可以:
template <typename T>
struct MyAllocList_typedef {
    typedef list<T, MyAlloc<T>> type;
};

void t3() {
    MyAllocList_using<int> lw;
    lw.push_back(1);
    MyAllocList_typedef<int>::type lw1;
    lw1.push_back(1);
}

// 使用typedef更复杂的情况, 类模板内部有一个链表
template <typename T>
class Widget {
private:
    // MyAllocList_typedef<T>::type 是带依赖型别,
    //  还需要加上typename声明符
    typename MyAllocList_typedef<T>::type list;
    /* MyAllocList_typedef<T>::type list; */
    // Missing 'typename' prior to dependent type name
    // 'MyAllocList_typedef<T>::type'
};

// 但是对于using类型声明, 就不需要这么复杂
template <typename T>
class Widget1 {
private:
    MyAllocList_using<T> list;
};

void t4() {
    Widget<int> w1;
    Widget1<int> w2;
}

// 关于 typedef 的新的问题, type 变量在某一个模板的特化版本中可能被使用了
class Wine {};
template <>
class MyAllocList_typedef<Wine> {
private:
    enum class WineType { White, Red, Green };
    WineType type; // type 此时作为一个数据成员
};

void t5() {
    MyAllocList_typedef<Wine> ww;
    //
}

void t6() {
    using a = long;
    /* using a = int; */
}

int main(int argc, char *argv[]) {
    /* t1(); */
    t3();
    return 0;
}
