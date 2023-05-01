#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

// 等价
template <typename T>
class P;
template <class T>
class P;

// typename 必须作为嵌套从属类型名称的前缀词
template <typename C>
void t1(const C& container) {
    if (container.size() >= 2) {
        // typename 必须加, 而且只能用typename
        typename C::const_iterator iter(container.begin());
        ++iter;
        int value = *iter;
        cout << value << endl;
    }
}

template <typename C>
void t2(const C& container, typename C::iterator it);

// 一个例外, typename不可以出现在Base classes list内的嵌套从属类型名称之前,
// 也不可以在成员初值列中作为Base class修饰符
template <typename T>
class Base {
public:
    // 这里是一个嵌套类
    class Nested {
    public:
        Nested() {}

    private:
        T xx;
    };
};

template <typename T>
class Derived : public Base<T>::Nested {
public:
    // init-list中不能加typename
    explicit Derived(int x) : Base<T>::Nested(x) {
        // 基类定义需要加
        typename Base<T>::Nested tmp;
    }
};

// 函数模板中的
template <typename IterT>
void workWithIterator(IterT iter) {
    /* typename iterator_traits<IterT>::value_type tmp(*iter); */
    typedef typename iterator_traits<IterT>::value_type VT;
    VT tmp(*iter);
}

int main(int argc, char* argv[]) {
    t1(vector<int>(3, 2));
    return 0;
}
