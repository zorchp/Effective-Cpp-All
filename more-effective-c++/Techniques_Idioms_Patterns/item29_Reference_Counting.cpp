#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>

namespace Base_version {
class String {
public:
    String(const char *value = "") {
        RealData = new char[strlen(value) + 1];
        strcpy(RealData, value);
    }
    ~String() {
        printf("call dtor\n");
        delete[] RealData;
    }
    String &operator=(const String &rhs) {
        if (this == &rhs)
            return *this;
        delete[] RealData;
        // 每次赋值都用新的地址
        RealData = new char[strlen(rhs.RealData) + 1];
        strcpy(RealData, rhs.RealData);
        return *this;
    }
    void printf_data() { //
        printf("%s\n", RealData);
    }
    void printf_data_address() { //
        printf("%p\n", RealData);
    }

private:
    char *RealData;
};
} // namespace Base_version

void t1() {
    using namespace Base_version;
    String a, b, c, d, e;
    a = b = c = d = e = "Hello";
    // 此时这几部分都是不同的地址
    a.printf_data(); //
    b.printf_data(); //
    c.printf_data();
    a.printf_data_address(); //
    b.printf_data_address(); //
    c.printf_data_address();
    // Hello
    // Hello
    // Hello
    // 0x106e005b0
    // 0x106e005d0
    // 0x106e005f0
}

namespace Base_ref_count_String {
class String {
public:
    String(const char *initValue = "") : value(new StringValue(initValue)) {
    }

    String(const String &rhs) {
        if (rhs.value->shareable) {
            value = rhs.value;
            ++value->refCount;
        } else { // 不可共享, 创建新的副本供写时复制使用
            value = new StringValue(rhs.value->RealData);
        }
    }
    String &operator=(const String &rhs) {
        if (value == rhs.value)
            return *this;
        if (--value->refCount == 0) // 递减当前对象的引用计数(左侧对象)
            delete value;

        value = rhs.value;
        ++value->refCount; // 递增右侧对象引用计数
        return *this;
    }

    const char &operator[](int index) const {
        return value->RealData[index];
    }
    char &operator[](int index) {
        if (value->refCount > 1) {
            --value->refCount;
            value = new StringValue(value->RealData);
        }
        // add this: 设置不可共享
        // 此时如果要修改, 那么就要创建新的副本
        value->shareable = false;
        return value->RealData[index];
    }

    ~String() {
        if (--value->refCount == 0)
            delete value;
    }
    void printf_data_address() {
        printf("%p\n", value);
    }

private:
    // 引用计数器实现
    struct StringValue {
        int refCount;
        bool shareable; // 标记共享状态
        char *RealData; // 实际数据存放
        StringValue(const char *initValue) : refCount(1), shareable(true) {
            RealData = new char[strlen(initValue) + 1];
            strcpy(RealData, initValue);
        }

        ~StringValue() {
            delete[] RealData;
        }
    };

    StringValue *value;
};
} // namespace Base_ref_count_String

void t2() {
    using namespace Base_ref_count_String;
    // 两个对象, 占用两份内存
    String s1("hello");
    String s2("hello");
    s1.printf_data_address();
    s2.printf_data_address();
    // 0x16f002ab8
    // 0x16f002ab0

    // 占用一份内存
    String s3("world");
    s3.printf_data_address();
    String s4 = s3;
    s4.printf_data_address();
    // 0x105700670
    // 0x105700670
}

void t3() {
    using namespace Base_ref_count_String;
    String s1 = "hello";
    char *p = &s1[1];
    String s2 = s1;
    // 需要设置共享变量
    printf("%c\n", *p);
    printf("s1[1]=%c, s2[1]=%c\n", s1[1], s2[1]);
    *p = 'f';
    printf("s1[1]=%c, s2[1]=%c\n", s1[1], s2[1]);
    // e
    // 由于设置了不可共享, 这里分成了两份内容
    // s1[1]=e, s2[1]=e
    // s1[1]=f, s2[1]=e
}

namespace Better_RC_String { // 类的实现者层面, 直接拿来用即可
// 抽象出来的 引用计数基类, 用于之后封装
class RCObject {
public:
    RCObject() : refCount(0), shareable(true) {
    } // 注意这里初始化成 0 了
    RCObject(const RCObject &) : refCount(0), shareable(true) {
    }
    RCObject &operator=(const RCObject &) {
        return *this;
    }
    virtual ~RCObject() = 0;

    void addReference() {
        ++refCount;
    }
    void removeReference() {
        if (--refCount == 0)
            delete this;
    }
    void markUnshareable() {
        shareable = false;
    }
    bool isShareable() const {
        return shareable;
    }
    bool isShared() const {
        return refCount > 1;
    }

private:
    int refCount;
    bool shareable;
};

RCObject::~RCObject() {
    printf("~RCObject()\n");
}

// 新的嵌套类实现引用计数字符串类型
// 自动完成引用计数, T必须继承自RCObject, 以使用引用计数的功能
template <class T>
class RCPtr { // 智能指针实现
public:
    RCPtr(T *realPtr = nullptr) : pointee(realPtr) {
        inc_cnt();
    }
    RCPtr(const RCPtr &rhs) : pointee(rhs.pointee) {
        // 当 String 发生拷贝初始化, 调用智能指针的拷贝构造函数
        inc_cnt();
    }
    ~RCPtr() {
        if (pointee) {
            pointee->removeReference();
            printf("~RCPtr(), removeReference\n");
        }
    }

    RCPtr &operator=(const RCPtr &rhs) { // COW call this
        if (pointee != rhs.pointee) {
            if (pointee)
                pointee->removeReference();
            pointee = rhs.pointee;
            inc_cnt();
        }
        return *this;
    }

    T *operator->() const {
        return pointee;
    }
    T &operator*() const {
        return *pointee;
    }

private:
    T *pointee;
    void inc_cnt() {
        if (pointee == nullptr)
            return;
        if (pointee->isShareable() == false) // 不可共享, 创建一份新的
            pointee = new T(*pointee);
        pointee->addReference(); // 默认值是 0, 这里初始化就要先加上一
    }
};

// 完整的实现, 用新封装好的 RCPtr 和 RCObject 实现
class String {
public:
    String(const char *initValue = "") : value(new StringValue(initValue)) {
    }

    const char &operator[](int index) const { // 直接取出值
        return value->data[index];
    }

    char &operator[](int index) { // COW, 如果可共享, 拷贝新值
        if (value->isShared()) {
            value = new StringValue(value->data);
        }
        value->markUnshareable();
        return value->data[index];
    }

    // for test
    void printf_data() {
        printf("%s\n", value->data);
    }
    void printf_data_address() {
        printf("%p\n", value->data);
    }

private:
    struct StringValue : public RCObject { // T 继承自 RCObject
        char *data;
        void init(const char *initValue) {
            data = new char[strlen(initValue) + 1];
            strcpy(data, initValue);
        }

        StringValue(const StringValue &rhs) {
            init(rhs.data);
        }
        StringValue(const char *initValue) {
            init(initValue); // deep copy
        }
        ~StringValue() {
            printf("~StringValue()\n");
            delete[] data;
        }
    };
    RCPtr<StringValue> value; // 接管数据
};
} // namespace Better_RC_String

void t4() {
    //
    using namespace Better_RC_String;
    String s = "abc";
    String s1 = s; // call copy-ctor
    s1[0] = 'b'; // COW, 需要生成一份对象, 然后智能指针删除原始对象
    s.printf_data_address();
    s1.printf_data_address();
    s.printf_data();
    s1.printf_data();
    // 之所以会产生这条, 是因为s1[0] = 'b';这条语句事实上调用了 RCPtr
    // 的拷贝赋值运算符(由于 COW 技术), 这就使得原对象出现了析构
    // ~RCPtr()
    // 0x1058006f0
    // 0x1058006d0
    // abc
    // bbc
    // ~StringValue()
    // ~RCObject()
    // ~RCPtr()
    // ~StringValue()
    // ~RCObject()
    // ~RCPtr()
}

namespace Best_RC {
class RCObject {
public:
    RCObject() : refCount(0), shareable(true) {
    } // 注意这里初始化成 0 了
    RCObject(const RCObject &) : refCount(0), shareable(true) {
    }
    RCObject &operator=(const RCObject &) {
        return *this;
    }
    virtual ~RCObject() = 0;

    void addReference() {
        ++refCount;
        printf("inc, now cnt=%d\n", refCount);
    }
    void removeReference() {
        --refCount;
        printf("dec, now cnt=%d\n", refCount);
        if (refCount == 0) {
            delete this;
        }
    }
    void markUnshareable() {
        shareable = false;
    }
    bool isShareable() const {
        return shareable;
    }
    bool isShared() const {
        return refCount > 1;
    }

    // private:
    int refCount;
    bool shareable;
};

RCObject::~RCObject() {
    printf("~RCObject()\n");
}

// 为库函数内的类实现引用计数(不可改动的类)Indirect
template <class T>
class RCIPtr {
public:
    RCIPtr(T *realPtr = nullptr) : counter(new CountHolder) {
        printf("%s\n", __FUNCTION__);
        counter->pointee = realPtr;
        inc_cnt();
    }

    RCIPtr(const RCIPtr &rhs) : counter(rhs.counter) {
        printf("%s\n", __FUNCTION__);
        inc_cnt();
    }

    ~RCIPtr() {
        printf("%s\n", __FUNCTION__);
        counter->removeReference();
    }
    RCIPtr &operator=(const RCIPtr &rhs) {
        printf("%s\n", __FUNCTION__);
        if (counter != rhs.counter) {
            counter->removeReference();
            counter = rhs.counter;
            inc_cnt();
        }
        return *this;
    }

    const T *operator->() const {
        return counter->pointee;
    }
    T *operator->() {
        makeCopy(); // COW
        return counter->pointee;
    }

    const T &operator*() const {
        return *(counter->pointee);
    }
    T &operator*() {
        makeCopy(); // COW
        return *(counter->pointee);
    }
    int get_cnt() const {
        return counter->refCount;
    }

private:
    struct CountHolder : public RCObject {
        ~CountHolder() {
            printf("%s\n", __FUNCTION__);
            delete pointee;
        }
        T *pointee;
    };
    CountHolder *counter; // 指向引用计数基类
    void inc_cnt() {
        if (counter->isShareable() == false) { // 需要写时复制
            printf("%s\n", __FUNCTION__);
            T *oldValue = counter->pointee;
            counter = new CountHolder;
            counter->pointee = new T(*oldValue);
        }
        counter->addReference();
    }
    void makeCopy() {
        if (counter->isShared()) { // 正在被共享
            printf("%s\n", __FUNCTION__);
            T *oldValue = counter->pointee;
            counter->removeReference();
            counter = new CountHolder;
            counter->pointee = new T(*oldValue);
            counter->addReference();
        }
    }
};

// 目标对象, 不可改动
class Widget {
public:
    Widget() = default;
    Widget(int size) {
    }
    Widget(const Widget &) {
    }
    ~Widget() {
    }
    // Widget &operator=(const Widget &);
    void doThis() {
        printf("%s\n", __FUNCTION__);
    }
    int showThat() const {
        printf("%s\n", __FUNCTION__);
        return 0;
    }
};

// 中间层
class RCWidget { // 充当了String 的角色, 内部以智能指针控制内存行为
public:
    RCWidget() = default;
    RCWidget(int size) : value(new Widget(size)) {
    }
    void doThis() {
        value->doThis();
    }
    int showThat() const {
        return value->showThat();
    }

private:
    RCIPtr<Widget> value; // 实际控制
};

} // namespace Best_RC

class P {
public:
    P() {
        printf("%s\n", __FUNCTION__);
    }
    void f() { //
        printf("%s\n", __FUNCTION__);
    }
    ~P() {
        printf("%s\n", __FUNCTION__);
    }
    int val;
};
// 使用: 改善效率的最佳时机
//  1. 相对多数的对象共享相对少量的实值
//  2. 对象实值的产生或销毁成本很高, 或者使用内存较大
void t5() {
    using namespace Best_RC;
    // printf("sizeof RCIPtr=%ld\n", sizeof(RCIPtr<P>)); // 8
    RCIPtr<P> p(new P); // p: 0->1
    auto q(p);          // q: 0->1->2, p: 1->2

    // printf("q.cnt=%d\n", q.get_cnt());
    // assert(q.get_cnt() == p.get_cnt()); // 2

    RCIPtr<P> r(new P); // r: 0->1
    r = q;              // r: 1->0, q: 2->3
} // 3->0, 这里用到了 RAII, 比较巧妙

int main(int argc, char *argv[]) {
    // t1();
    // t2();
    // t3();
    // t4();
    t5();
    return 0;
}
