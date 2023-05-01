#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sys/signal.h>

using namespace std;

class String {
public:
    String(const char *value = "") : data1((char *)value) {}
    String &operator=(const String &rhs) {
        if (this == &rhs) return *this;
        delete[] data1;
        data1 = new char[strlen(rhs.data1) + 1];
        strcpy(data1, rhs.data1);
        return *this;
    }

private:
    char *data1;
};

void t1() {
    String a, b, c, d, e;
    a = b = c = d = e = "Hello";
}


class String_RC {
public:
    String_RC(const char *initValue = "");
    /* String_RC(const String_RC &rhs) : value(rhs.value) {} */
    String_RC(const String_RC &rhs);
    String_RC &operator=(const String_RC &rhs);

    const char &operator[](int index) const;
    char &operator[](int index);

    ~String_RC() {
        if (--value->refCount == 0) delete value;
    }

private:
    // 引用计数器实现
    struct StringValue {
        int refCount;
        bool shareable; // 标记共享状态
        char *data1;
        StringValue(const char *initValue);
        ~StringValue() { delete[] data1; }
    };

    StringValue *value;
};

String_RC::String_RC(const String_RC &rhs) {
    if (rhs.value->shareable) {
        value = rhs.value;
        ++value->refCount;
    } else { // 不可共享, 创建新的副本供写时复制使用
        value = new StringValue(rhs.value->data1);
    }
}

String_RC::StringValue::StringValue(const char *initValue)
    : refCount(1), shareable(true) {
    data1 = new char[strlen(initValue) + 1];
    strcpy(data1, initValue);
}
String_RC::String_RC(const char *initValue)
    : value(new StringValue(initValue)) {}

String_RC &String_RC::operator=(const String_RC &rhs) {
    if (value == rhs.value) return *this;
    if (--value->refCount == 0) delete value;

    value = rhs.value;
    ++value->refCount;
    return *this;
}

const char &String_RC::operator[](int index) const {
    return value->data1[index];
}
char &String_RC::operator[](int index) {
    if (value->refCount > 1) {
        --value->refCount;
        value = new StringValue(value->data1);
    }
    // add this: 设置不可共享
    // 此时如果要修改, 那么就要创建新的副本
    value->shareable = false;
    return value->data1[index];
}


void t2() {
    //
    // 两个对象, 占用两份内存
    String_RC s1("hello");
    String_RC s2("hello");
    // 占用一份内存
    String_RC s3("world");
    String_RC s4 = s3;
}

void t3() {
    String_RC s1 = "hello";
    char *p = &s1[1];
    String_RC s2 = s1;
    // 需要设置共享变量
    cout << *p << endl;                    // e
    cout << s1[1] << " " << s2[1] << endl; // e e
    *p = 'f';
    cout << s1[1] << " " << s2[1] << endl; // f e
}

// 引用计数基类
class RCObject {
public:
    RCObject() : refCount(0), shareable(true) {}
    RCObject(const RCObject &) : refCount(0), shareable(true) {}
    RCObject &operator=(const RCObject &) { return *this; }
    virtual ~RCObject() = 0;

    void addReference() { ++refCount; }
    void removeReference() {
        if (--refCount == 0) delete this;
    }
    void markUnshareable() { shareable = false; }
    bool isShareable() const { return shareable; }
    bool isShared() const { return refCount > 1; }

private:
    int refCount;
    bool shareable;
};

RCObject::~RCObject() {}

// 新的嵌套类实现引用计数字符串类型
class String1 {
private:
    struct StringValue : public RCObject {
        char *data;
        StringValue(const char *initValue) {
            data = new char[strlen(initValue) + 1];
            strcpy(data, initValue);
        }
        ~StringValue() { delete[] data; }
    };
};


// 自动完成引用计数, T必须继承自RCObject
template <class T>
class RCPtr {
public:
    RCPtr(T *realPtr = 0) : pointee(realPtr) { init(); }
    RCPtr(const RCPtr &rhs) : pointee(rhs.pointee) { init(); }
    ~RCPtr() {
        if (pointee) pointee->removeReference();
    }

    RCPtr &operator=(const RCPtr &rhs) {
        if (pointee != rhs.pointee) {
            if (pointee) pointee->removeReference();
            pointee = rhs.pointee;
            init();
        }
        return *this;
    }

    T *operator->() const { return pointee; }
    T &operator*() const { return *pointee; }

private:
    T *pointee;
    void init() {
        if (pointee == 0) return;
        if (pointee->isShareable() == false) pointee = new T(*pointee);
        pointee->addReference();
    }
};

class String2 {
public:
    String2(const char *initValue = "") : value(new StringValue(initValue)) {}

    const char &operator[](int index) const { return value->data[index]; }
    char &operator[](int index) {
        if (value->isShared()) value = new StringValue(value->data);
        value->markUnshareable();
        return value->data[index];
    }

private:
    struct StringValue : public RCObject {
        char *data;
        void init(const char *initValue) {
            data = new char[strlen(initValue) + 1];
            strcpy(data, initValue);
        }

        StringValue(const StringValue &rhs) { init(rhs.data); }
        StringValue(const char *initValue) {
            init(initValue); // 深拷贝
        }
        ~StringValue() { delete[] data; }
    };
    RCPtr<StringValue> value;
};

// 为库函数内的类实现引用计数(不可改动的类)Indirect
template <class T>
class RCIPtr {
public:
    RCIPtr(T *realPtr = 0) : counter(new CountHolder) {
        counter->pointee = realPtr;
        init();
    }

    RCIPtr(const RCIPtr &rhs) : counter(rhs.counter) { init(); }

    ~RCIPtr() { counter->removeReference(); }
    RCIPtr &operator=(const RCIPtr &rhs) {
        if (counter != rhs.counter) {
            counter->removeReference();
            counter = rhs.counter;
            init();
        }
        return *this;
    }

    const T *operator->() const { return counter->pointee; }
    T *operator->() {
        makeCopy();
        return counter->pointee;
    }

    const T &operator*() const { return *(counter->pointee); }
    T &operator*() {
        makeCopy();
        return *(counter->pointee);
    }

private:
    struct CountHolder : public RCObject {
        ~CountHolder() { delete pointee; }
        T *pointee;
    };
    CountHolder *counter;
    void init();
    void makeCopy();
};

template <class T>
void RCIPtr<T>::init() {
    if (counter->isShareable() == false) {
        T *oldValue = counter->pointee;
        counter = new CountHolder;
        counter->pointee = new T(*oldValue);
    }
    counter->addReference();
}

template <class T>
void RCIPtr<T>::makeCopy() {
    if (counter->isShared()) {
        T *oldValue = counter->pointee;
        counter->removeReference();
        counter = new CountHolder;
        counter->pointee = new T(*oldValue);
        counter->addReference();
    }
}

// 目标对象, 不可改动
class Widget {
public:
    Widget() = default;
    Widget(int size) {}
    Widget(const Widget &) {}
    ~Widget() {}
    Widget &operator=(const Widget &);
    void doThis();
    int showThat() const;
};

// 中间层
class RCWidget {
public:
    RCWidget() = default;
    RCWidget(int size) : value(new Widget(size)) {}
    void doThis() { value->doThis(); }
    int showThat() const { return value->showThat(); }

private:
    RCIPtr<Widget> value;
};

// 使用: 改善效率的最佳时机
//  1. 相对多数的对象共享相对少量的实值
//  2. 对象实值的产生或销毁成本很高, 或者使用内存较大
void t4() {
    RCWidget w1, w2;
    w1 = w2;
}

int main(int argc, char *argv[]) {
    /* t1(); */
    /* t2(); */
    /* t3(); */
    t4();
    return 0;
}
