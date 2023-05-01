#include <set>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <iostream>
#include <cstring>

using namespace std;

int ciCharCompare(char c1, char c2) {
    int lc1 = tolower(static_cast<unsigned char>(c1)); // maybe signed char
    int lc2 = tolower(static_cast<unsigned char>(c2));
    if (lc1 < lc2) return -1;
    if (lc1 > lc2) return 1;
    return 0;
}

int ciStringCompareImpl(const string &, const string &);
int ciStringCompare(const string &s1, const string &s2) {
    if (s1.size() <= s2.size())
        return ciStringCompareImpl(s1, s2);
    else
        return -ciStringCompareImpl(s2, s1);
}

int ciStringCompareImpl(const string &s1, const string &s2) {
    typedef pair<string::const_iterator, string::const_iterator> PSCI;

    PSCI p = mismatch(s1.begin(), s1.end(), s2.begin(), not_fn(ciCharCompare));
    // return 0: s1==s2
    // otherwise: s1.size()<s2.size()
    if (p.first == s1.end()) return (p.second == s2.end()) ? 0 : -1;

    return ciCharCompare(*p.first, *p.second);
}


struct CaseIgnoreStringCompare : public function<bool(string, string)> {
    bool operator()(const string &lhs, const string &rhs) const {
        return ciStringCompare(lhs, rhs);
    }
};
// or use this
bool ciCharLess(char c1, char c2) {
    return tolower(static_cast<unsigned char>(c1)) <
           tolower(static_cast<unsigned char>(c2));
}

bool ciStringCompare1(const string &s1, const string &s2) {
    return lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(),
                                   ciCharLess);
}


bool ciStringCompare2(const string &s1, const string &s2) {
    // with c str library
    // return stricmp(s1.c_str(), s2.c_str());
    return true;
}

void t1() { //
    set<string, CaseIgnoreStringCompare> ciss;
    ciss.insert("pass");
    ciss.insert("Pass");
    for (auto i : ciss) cout << i << endl; // pass
    // 只能使用成员函数版本的find
    auto it = ciss.begin();
    // 成员函数find首先使用自定义的比较规则(等价)
    if ((it = ciss.find("Pass")) != ciss.end()) cout << *it << endl; // pass
    // error
    if ((it = find(ciss.begin(), ciss.end(), "Pass")) != ciss.end())
        cout << *it << endl;
    else
        cout << "cannot find by ::find()\n"; //
    // cannot find by ::find(), 全局find()采用(值的)相等来比较
}


int main(int argc, char const *argv[]) {
    t1();
    return 0;
}