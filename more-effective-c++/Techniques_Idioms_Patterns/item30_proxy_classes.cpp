#include <iostream>
#include <cstring>

using namespace std;

void create_2darray(int d1, int d2) {
    int data[d1][d2]; // ok
    /* int *data1 = new int[d1][d2]; */
    // error: array size is not a constant expression
}

void t1() {
    int data[20][20]; // ok
    create_2darray(1, 2);
}

template <class T>
class Array_2Dim {
public:
    Array_2Dim(int dim1, int dim2) {}
};

void create_by_template(int d1, int d2) {
    Array_2Dim<int> data(d1, d2);               // ok
    auto data1 = new Array_2Dim<float>(d1, d2); // ok
}

void t2() {
    /* Array_2Dim<int> data(10, 20); */
    /* Array_2Dim<double> *data1 = new Array_2Dim<double>(20, 30); */
}

// 重载两次(嵌套的)operator[]完成二维数组创建操作
template <class T>
class Array2D {
public:
    Array2D(int d1, int d2) {}
    // 此即代理类
    class Array1D {
    public:
        T &operator[](int index);
        const T &operator[](int index) const;
    };
    Array1D operator[](int index);
    const Array1D operator[](int index) const;
};

void t3() {
    /* Array2D<int> data(10, 20); */
    /* cout << data[3][5] << endl; */
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


// 引用计数的string类
class String {
public:
    class CharProxy {
    public:
        CharProxy(String &str, int index) : theString(str), charIndex(index) {}
        CharProxy &operator=(const CharProxy &rhs) {
            // 如果共享同一个实值, 复制一份新的, 仅供本字符串使用
            // 然后在拷贝赋值运算符中进行写操作
            if (theString.value->isShared())
                theString.value = new StringValue(theString.value->data);

            theString.value->data[charIndex] =
                rhs.theString.value->data[rhs.charIndex];
            return *this;
        }
        CharProxy &operator=(char c) {
            // 这两个拷贝赋值运算符代码中重复的部分可以写一个新函数init来避免重复
            if (theString.value->isShared())
                theString.value = new StringValue(theString.value->data);
            theString.value->data[charIndex] = c;
            return *this;
        }
        // 隐式类型转换, 方便cout
        operator char() const { return theString.value->data[charIndex]; }

        // 这里为了确保取地址运算符正确性(返回的代理类转换为char*),
        // 加上&的操作符重载
        char *operator&() {
            if (theString.value->isShared())
                theString.value = new StringValue(theString.value->data);
            theString.value->markUnshareable();
            return &(theString.value->data[charIndex]);
        }
        const char *operator&() const {
            return &(theString.value->data[charIndex]);
        }

    private:
        String &theString;
        int charIndex;
    };

    // 这里是一个不同点, 返回的并不是原始的String对象, 而是一个代理类生成的对象
    // 没有动作施加于返回的字符, 可以采用缓式评估提高性能
    const CharProxy operator[](int index) const {
        return CharProxy(const_cast<String &>(*this), index);
    }
    CharProxy operator[](int index) { return CharProxy(*this, index); }

    // 这里因为要处理String的私有成员value, 所以将稍早出现的代理类设置为friend
    friend class CharProxy;

    String(const char *initValue = "") : value(new StringValue(initValue)) {}


private:
    struct StringValue : public RCObject {
        char *data;
        void init(const char *initValue) {
            // 默认一个字符大小
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

void t4() {
    String s1, s2("19urhf");
    cout << s2[5] << endl; // f
    s2[5] = 'x';
    cout << s2[5] << endl; // x
    s1[0] = s2[5];
    cout << s1[0] << endl; // x
}

void t5() {
    String s1 = "Hello";
    // error, 没有对应的转换函数
    char *p = &s1[1];
    cout << *p << endl; // e
}

// 引用计数数组
template <class T>
class Array {
public:
    class Proxy {
    public:
        Proxy(Array<T> &array, int index);
        Proxy &operator=(const T &rhs);
        operator T() const;
    };

    const Proxy operator[](int index) const;
    Proxy operator[](int index);
};

void t6() {
    Array<int> intArray;
    intArray[5] = 22;
    /* intArray[5] += 1; // error, 没有对应的重载函数 */
    /* intArray[5]++; // error, 没有对应的重载函数 */
    // 重载每一个运算符, 需要的工作量巨大
    // 另一方面, 通过代理类调用真实的成员函数会失败
}

int main(int argc, char *argv[]) {
    /* t1(); */
    /* t4(); */
    /* t5(); */
    t6();
    return 0;
}
