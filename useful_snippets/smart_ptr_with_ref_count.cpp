class RCObject {
public:
    RCObject() : refCount(0), shareable(true) {
    }
    RCObject(const RCObject &) : refCount(0), shareable(true) {
    }
    RCObject &operator=(const RCObject &) {
        return *this;
    }
    virtual ~RCObject() = 0; // 只能被用作基类

    void addReference() {
        ++refCount;
    }
    void removeReference() {
        if (--refCount == 0)
            delete this;
    }
    void markUnshareable() { // 标记为不可共享
        shareable = false;
    }
    bool isShareable() const { // 是否可共享
        return shareable;
    }
    bool isShared() const { // 是否正在被共享
        return refCount > 1;
    }

private:
    int refCount;   // 引用计数值
    bool shareable; // 对象是否可共享
};

RCObject::~RCObject() {
}


// 为库函数内的类实现引用计数(不可改动的类)Indirect
template <class T>
class RCIPtr {
public:
    RCIPtr(T *realPtr = 0) : counter(new CountHolder) {
        counter->pointee = realPtr;
        init();
    }

    RCIPtr(const RCIPtr &rhs) : counter(rhs.counter) {
        init();
    }

    ~RCIPtr() {
        counter->removeReference();
    }
    RCIPtr &operator=(const RCIPtr &rhs) {
        if (counter != rhs.counter) {
            counter->removeReference();
            counter = rhs.counter;
            init();
        }
        return *this;
    }

    const T *operator->() const {
        return counter->pointee; // const 访问, 不需要 COW
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

private:
    struct CountHolder : public RCObject {
        ~CountHolder() {
            delete pointee;
        }
        T *pointee;
    };
    CountHolder *counter;
    void init() {
        if (counter->isShareable() == false) { // 不可共享,创造新值
            T *oldValue = counter->pointee;
            counter = new CountHolder;
            counter->pointee = new T(*oldValue);
        }
        counter->addReference();
    }
    void makeCopy() {              // COW
        if (counter->isShared()) { // 正在共享, 先复制一份
            T *oldValue = counter->pointee;
            counter->removeReference();
            counter = new CountHolder;
            counter->pointee = new T(*oldValue);
            counter->addReference();
        }
    }
};
