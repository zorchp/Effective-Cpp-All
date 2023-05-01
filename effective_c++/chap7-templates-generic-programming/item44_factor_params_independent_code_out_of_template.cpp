#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;

// n 是非类型参数
template <typename T, size_t n>
class SquareMatrix {
public:
    void invert() { cout << "invert()\n"; }
};

void t1() {
    // 这会具现化两份代码, 但是只是非类型参数不同
    // 于是导致了代码膨胀
    SquareMatrix<double, 5> sm1;
    sm1.invert();
    SquareMatrix<double, 5> sm2;
    sm2.invert();
}

// 第二种不会导致代码膨胀的实现,
// 但是需要定制参数使得派生类知道操作哪一块数据(通过指针实现)
template <typename T>
class SquareMatrixBase {
protected:
    SquareMatrixBase(size_t n, T* mem) : size(n), pData(mem) {}
    // re-assignment
    void setDataPtr(T* ptr) { pData = ptr; }
    void invert(size_t maxSize) { cout << maxSize << " invert()\n"; }

private:
    size_t size;
    T* pData;
};

template <typename T, size_t n>
class SquareMatrix1 : private SquareMatrixBase<T> {

public:
    SquareMatrix1() : SquareMatrixBase<T>(n, data) {}
    void invert() { this->invert(n); }

private:
    T data[n * n];
};

int main(int argc, char* argv[]) {
    t1();
    return 0;
}
