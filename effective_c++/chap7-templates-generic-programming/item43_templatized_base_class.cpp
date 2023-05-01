#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

class A {
public:
    void send_clear_text(const string &msg);
    void send_encrypted(const string &msg);
};
class B {
public:
    void send_clear_text(const string &msg);
    void send_encrypted(const string &msg);
};

class MsgInfo {};

template <typename Company>
class MsgSender {
public:
    void SendClear(const MsgInfo &info) {
        string msg;
        Company c;
        c.send_clear_text(msg);
    }

    void sendSecret(const MsgInfo &info) {}
};

template <typename Company>
class LogMsgSender : public MsgSender<Company> {
public:
    void sendClearMsg(const MsgInfo &info) {
        // before
        // 类模板并不知道继承的是哪个类, 例如下面的全特化就未提供此函数
        /* SendClear(info); */
        // 第一种方法: this指针 (需要确保被调用的方法被继承下来)
        /* this->SendClear(info); */
        // 第二种方法: using 声明
        /* using MsgSender<Company>::SendClear; */
        /* SendClear(info); */
        // 第三种方法: 指定父类
        MsgSender<Company>::SendClear(info);
        // after
    }
};

class CompanyZ {
public:
    void send_encrypted(const string &msg) {}
};

// 全特化
template <>
class MsgSender<CompanyZ> {
public:
    void sendSecret(const MsgInfo &info) {}
};

void t1() {
    //
}

int main(int argc, char *argv[]) {
    t1();
    return 0;
}
