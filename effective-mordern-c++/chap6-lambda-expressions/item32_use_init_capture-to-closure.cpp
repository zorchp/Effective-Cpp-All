#include <iostream>
#include <memory>
#include <functional>
#include <vector>

using namespace std;

class Widget {
public:
    bool isValidated() const;

private:
    //
};


void t1() {
    //
    auto pw = make_unique<Widget>();
    // 初始化捕获(位于=左侧 的是指定的比堡垒成员变量名, 右侧的是初始化表达式)
    // 左侧是闭包类的作用域, 右侧的是lambda加以定义之处的作用域
    auto func = [pw = std::move(pw)] { return pw->isValidated(); };
}

// 使用C++11 的话:
class IsVal {
public:
    using DataType = unique_ptr<Widget>;
    explicit IsVal(DataType&& ptr) : pw(std::move(ptr)) {}

    bool operator()() const { return pw->isValidated(); }

private:
    DataType pw;
};

auto func = IsVal(make_unique<Widget>());

void t2() {
    // C++14
    vector<double> data;
    auto func1 = [data = std::move(data)] {
        // use data
    };
    // C++11 use bind:
    auto func2 = std::bind([](const vector<double>& data) { /*use data */ },
                           std::move(data));
}
void t2_mutable() {
    // C++14
    vector<double> data;
    auto func1 = [data = std::move(data)]() mutable {
        // use data
    };
    // C++11 use bind:
    auto func2 =
        std::bind([](const vector<double>& data) mutable { /*use data */ },
                  std::move(data));
}


void t3() {
    // c++14
    auto f1 = [pw = make_unique<Widget>()] { return pw->isValidated(); };

    // c++11 with bind
    auto f2 =
        std::bind([](unique_ptr<Widget>& pw) { return pw->isValidated(); },
                  make_unique<Widget>());
}

int main(int argc, char* argv[]) {
    /* t1(); */
    t2();
    return 0;
}
