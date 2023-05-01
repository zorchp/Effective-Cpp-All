#include <iostream>
#include <vector>
#include <string>
#include <array>

using namespace std;

// vector 内容都是存放在堆中的, 所以move操作只移动指针即可,
// 但是array是把整体作为一个部分放在栈区的, 所以移动操作成本很高
class P {};
void t1() {
    vector<P> vp1;
    // 常数时间
    auto vp2 = std::move(vp1);

    array<P, 1000> ap1;
    // 线性时间
    auto ap2 = std::move(ap1);
    // 还要考虑P对象本身是移动还是复制速度更快
}

void t2() {
    // 针对字符串来说, string容器是常数时间的移动和线性时间的复制,
    // 考虑SSO(小型字符串优化), 使得字符串都存放在string对象某一个缓冲区中,
    // 虽然提高了读取等速度, 但是此时移动(小型字符串的)速度也为线性了
}

// 下面的场景, 移动操作并不会带来什么好处:
// 1. 没有移动操作
// 2. 移动未能更快
// 3. 移动不可用(可能的情况: 移动本可以发生, 要求移动操作不可发射异常,
// 但该操作未加上noexcept 声明)
// 4. 源对象是左值, 只有右值可以作为移动操作的源
