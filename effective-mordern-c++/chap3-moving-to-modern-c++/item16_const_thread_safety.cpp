#include <iostream>
#include <mutex>
#include <atomic>
#include <cmath>
#include <vector>

using namespace std;
// 应该保证const成员函数具有线程安全性

class Polynomial {
public:
    using RootType = vector<double>;
    RootType roots() const {
        lock_guard<mutex> g(mtx);
        if (!rootsAreValid) {
            //
            rootsAreValid = true;
        }
        return rootVals;
    } // 此处解开互斥量(智能)

private:
    // 通过写入互斥量来保证线程安全
    mutable mutex mtx; // 仅支持搬移操作的型别
    mutable bool rootsAreValid{false};
    mutable RootType rootVals{};
};

void t1() {
    // 两个线程同时在一个Polynomial对象上调用roots
    Polynomial p;
    // 下面是两个线程执行(对于读操作, 线程安全)
    // 但是写操作会导致竞态条件
    auto roots0fp = p.roots();
    auto valsGivingZero = p.roots();
}

// 只移动属性, 会导致含于其中的类失去可复制性

// 第二种成本较低的方法, 使用原子操作
class Point {
public:
    double distanceFromOrigin() const noexcept {
        ++callCount;
        return sqrt((x * x) + (y * y));
    }

private:
    mutable atomic<unsigned> callCount{}; // 仅支持搬移操作的型别
    double x, y;
};

// 应该多使用atomic操作, 但是也会有一些顺序问题

int expensiveComputation1() { return 1; }
int expensiveComputation2() { return 2; }

class Widegt {
public:
    int magicValue() const {
        if (cacheValid)
            return cachedValue;
        else {
            auto val1 = expensiveComputation1();
            auto val2 = expensiveComputation2();
            // 下面两行交换也不行(会导致数据并未被赋值, 线程就结束了)
            cachedValue = val1 + val2;
            cacheValid = true;
            return cachedValue;
        }
    }

private:
    mutable atomic<bool> cacheValid{};
    mutable atomic<int> cachedValue{};
};

// case:
//  一个线程调用magicValue, 观察到cacheValid为false, 于是执行两个大开销的计算,
//  赋值给cachedValue
// 另一个线程调用magicValue, 同样发现cacheValid为false, 进行同样的运算

// 所以对于单一变量赋值的情况可以用atomic, 多个变量的情况只能采用互斥量了
// 准确来说, 是对于单个要求同步的变量或者内存区域, 使用atomic就足够了
// 但是如果有两个或者多个变量或内存区域需要作为一个整体进行操作,
// 那就只能使用互斥量了

class Widegt1 {
public:
    int magicValue() const {
        lock_guard<mutex> guard(mtx); // 添加互斥量
        if (cacheValid)
            return cachedValue;
        else {
            auto val1 = expensiveComputation1();
            auto val2 = expensiveComputation2();
            // 下面两行交换也不行(会导致数据并未被赋值, 线程就结束了)
            cachedValue = val1 + val2;
            cacheValid = true;
            return cachedValue;
        }
    } // 解除互斥量

private:
    mutable mutex mtx;
    mutable bool cacheValid{}; // 此时两个变量不再具有原子性
    mutable int cachedValue{};
};

int main(int argc, char *argv[]) {
    t1();
    return 0;
}
