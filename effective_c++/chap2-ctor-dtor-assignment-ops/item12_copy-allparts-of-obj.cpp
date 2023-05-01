#include <iostream>
#include <string>

using namespace std;

void logCall(const string& funcName) { cout << funcName << " called.\n"; }

class Date {};

class Customer {
public:
    Customer(const Customer& rhs) : name(rhs.name) {
        logCall("Customer copy-ctor");
    }
    Customer& operator=(const Customer& rhs) {
        logCall("Customer copy assignment");
        name = rhs.name;
        return *this;
    }

private:
    string name;
    /* Date last; // 如果新添加了一个数据成员,
     * 需要重写copy-ctor以及copy-assignment */
};

class PriorityCustormer : public Customer {
public:
    PriorityCustormer(const PriorityCustormer& rhs)
        : Customer(rhs), priority(rhs.priority) { // 必须加上对基类的copy-ctor
        logCall("PriorityCustormer copy-ctor");
    }
    PriorityCustormer& operator=(const PriorityCustormer& rhs) {
        logCall("PriorityCustormer copy-assignment");
        Customer::operator=(rhs); // 对基类的copy-assignment
        priority = rhs.priority;
        return *this;
    }

private:
    int priority;
};

void t1() {}

int main(int argc, char* argv[]) {
    t1();
    return 0;
}
