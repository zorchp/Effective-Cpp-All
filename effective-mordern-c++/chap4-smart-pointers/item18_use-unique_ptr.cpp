#include <cstddef>
#include <iostream>
#include <memory>

using namespace std;
/*
为什么 auto_ptr 在 C++11 中被弃用了?
1. 因为 C++98 中还没有移动语义, 所以只能使用复制操作,
 而这样操作一个智能指针会引起: "对auto_ptr 对象执行复制操作会将其值置为空"
2. 不能在容器中存储auto_ptr对象
*/

class Investment {
public:
    // 为保证资源被释放(通过基类指针删除派生类对象)
    virtual ~Investment() {} // 必备的设计
};

// 继承的子类
class Stock : public Investment {
public:
    ~Stock() {}
};
class Bond : public Investment {};
class RealEstate : public Investment {};

// 自定义的析构器, 记录日志之后进行内存释放
auto delInvmt = [](Investment *pInvestment) {
    // make log entry
    delete pInvestment;
};

// 工厂函数, 指涉到根据指定实参创建的对象
template <typename... Ts>
// 这里的实参版本需要进行decltype推导
unique_ptr<Investment, decltype(delInvmt)> makeInvestment(Ts &&...params) {
    unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);
    // 满足对应的要求, 就进行创建
    if (1)
        // 完美转发, 可以使得所创建的对象的构造函数能获得调用者提供的所有信息
        pInv.reset(new Stock(std::forward<Ts>(params)...));
    else if (2)
        pInv.reset(new Bond(std::forward<Ts>(params)...));
    else if (3)
        pInv.reset(new RealEstate(std::forward<Ts>(params)...));
    return pInv;
}

// C++14版本, 简洁明了
template <typename... Ts>
auto makeInvestment1(Ts &&...params) {
    // 函数对象放在函数内
    auto delInvmt1 = [](Investment *pInvestment) {
        // make log entry
        delete pInvestment;
    };

    unique_ptr<Investment, decltype(delInvmt1)> pInv(nullptr, delInvmt1);
    // 满足对应的要求, 就进行创建
    if (1)
        pInv.reset(new Stock(std::forward<Ts>(params)...));
    else if (2)
        pInv.reset(new Bond(std::forward<Ts>(params)...));
    else if (3)
        pInv.reset(new RealEstate(std::forward<Ts>(params)...));
    return pInv;
}

// 如果使用函数作为自定义的析构器(需要保存函数指针), 导致内存占用增加
void delInvmt2(Investment *pInvestment) {
    // make log entry
    delete pInvestment;
}

// 此时返回值的大小等于Investment*的大小加上至少(因为函数内部可能保存状态)函数指针的大小
template <typename... Ts>
unique_ptr<Investment, void (*)(Investment *)> makeInvestment2(Ts &&...params);


// 与共享智能指针的互相转化非常方便
shared_ptr<Investment> sp = makeInvestment1();

void t1() {
    //
    auto pInvestment = makeInvestment();
    cout << sizeof(pInvestment) << endl; // 8
}

int main(int argc, char *argv[]) {
    t1();
    return 0;
}
