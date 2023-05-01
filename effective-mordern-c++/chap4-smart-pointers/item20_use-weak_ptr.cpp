#include <iostream>
#include <unordered_map>
#include <memory>
#include <cassert>

using namespace std;

/* 以下三种情况(用途), 使用weak_ptr(而不是shared_ptr)
1. 缓存
2. 观察者列表(观察者模式)
3. 避免 shared_ptr 出现的指针环路(循环引用)
*/

class Widget {
public:
    //
};

void t1() {
    // 检查空悬指针
    auto spw = make_shared<Widget>(); // 引用计数+1
    weak_ptr<Widget> wpw(spw);        // 引用计数仍为1
    spw = nullptr; // 引用计数变为0, Widget 被析构, wpw空悬
    cout << wpw.expired() << endl; // 1(true)
    //
    /* shared_ptr<Widget> spw1 = wpw.lock(); // 若wpw失效, spw1为空 */
    /* assert(spw1 == nullptr); */

    auto spw2 = wpw.lock(); // 此时spw2为空
    assert(spw2 == nullptr);
    shared_ptr<Widget> spw3(wpw); // 如果weak_ptr 失效, 抛出异常
}


// 用作缓存处理
class WidgetID {
public:
    //
    struct hash_name {
        size_t operator()(const WidgetID &p) const { return hash<int>()(p.id); }
    };
    bool operator==(const WidgetID &wid) const { return this->id == wid.id; }

private:
    int id;
};


unique_ptr<const Widget> loadWidget(WidgetID id) {
    return make_unique<Widget>();
}

shared_ptr<const Widget> fastLoadWidget(WidgetID id) {
    static unordered_map<WidgetID, weak_ptr<const Widget>, WidgetID::hash_name>
        cache;

    /* auto objPtr = cache[id].lock(); */
    // 如果对象不在缓存中, 返回空指针, objPtr指向待缓存对象
    shared_ptr<const Widget> objPtr = cache[id].lock();
    // 如果对象不在缓存中, 加载并缓存
    if (!objPtr) {
        objPtr = loadWidget(id);
        cache[id] = objPtr;
    }
    return objPtr;
}

// 观察者模式
std::weak_ptr<int> gw;

void observe() {
    std::cout << "gw.use_count() == " << gw.use_count() << "; ";
    // we have to make a copy of shared pointer before usage:
    if (std::shared_ptr<int> spt = gw.lock()) {
        std::cout << "*spt == " << *spt << '\n';
    } else {
        std::cout << "gw is expired\n";
    }
}

void t2() {
    {
        auto sp = std::make_shared<int>(42);
        gw = sp;

        observe();
    }

    observe();
    /*
    gw.use_count() == 1; *spt == 42
    gw.use_count() == 0; gw is expired
    */
}

namespace case1 {
class A;
class B {
public:
    B() { cout << __FUNCTION__ << endl; }
    ~B() { cout << __FUNCTION__ << endl; }
    shared_ptr<A> spa;
};

class A {
public:
    A() { cout << __FUNCTION__ << endl; }
    ~A() { cout << __FUNCTION__ << endl; }
    shared_ptr<B> spb;
};
// A 和 C 共享 B 的所有权
class C {
public:
    C() { cout << __FUNCTION__ << endl; }
    ~C() { cout << __FUNCTION__ << endl; }

    shared_ptr<B> spb;
};
} // namespace case1
void t3() {
    using namespace case1;
    auto a = make_shared<A>();
    auto b = make_shared<B>();
    a->spb = b;
    b->spa = a;
    cout << a.use_count() << endl;
    cout << b.use_count() << endl;
    /* A */
    /* B */
    /* 2 */
    /* 2 */
    // 析构函数未执行
}

namespace case2 {

class A;
class B {
public:
    B() { cout << __FUNCTION__ << endl; }
    ~B() { cout << __FUNCTION__ << endl; }
    weak_ptr<A> spa;
};

class A {
public:
    A() { cout << __FUNCTION__ << endl; }
    ~A() { cout << __FUNCTION__ << endl; }
    weak_ptr<B> spb;
};
// A 和 C 共享 B 的所有权
class C {
public:
    C() { cout << __FUNCTION__ << endl; }
    ~C() { cout << __FUNCTION__ << endl; }

    weak_ptr<B> spb;
};
} // namespace case2

void t4() {
    using namespace case2;
    auto a = make_shared<A>();
    auto b = make_shared<B>();
    a->spb = b;
    b->spa = a;
    cout << a.use_count() << endl;
    cout << b.use_count() << endl;
    /* A */
    /* B */
    /* 1 */
    /* 1 */
    /* ~B */
    /* ~A */
}

int main(int argc, char *argv[]) {
    /* t1(); */
    /* t2(); */
    /* t3(); */
    t4();
    return 0;
}
