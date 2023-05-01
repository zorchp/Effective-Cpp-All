#include <iostream>
using namespace std;

class EP {
public:
    int ID;
    EP(int num) : ID(num) { cout << "call EP(int), " << num << "\n"; }
    ~EP() { cout << ID << "deleted...\n"; }
};


void t1() {
    //
    void *rawMemory = operator new[](100 * sizeof(EP));
    EP *best = static_cast<EP *>(rawMemory);
    cout << "rawMemory=" << hex << rawMemory << endl;
    cout << "best=" << hex << best << endl;
    // rawMemory=0x13ee043e0
    // best=0x13ee043e0
    // placement new:
    for (int i{}; i < 10; ++i) new (&best[i]) EP(i);

    cout << "access :\n";
    for (int i{}; i < 10; ++i) cout << best[i].ID << endl;

    cout << "delete them:\n";
    for (int i{}; i < 10; ++i) best[i].~EP();
    // for (int i{9}; i >= 0; --i) best[i].~EP();
    operator delete[](rawMemory);

    // delete[] best;
    // undefined, 因为best并非来自new, 而是operator new(需要一一对应)
}

void t2() {
    void *buf = new int[100];
    EP *np = static_cast<EP *>(buf);
    // placement new : operator new (size_t, void* start);
    new (&np[0]) EP(100);
    new (&np[1]) EP(200);
    np[1].~EP();
    np[0].~EP();
    // operator delete[](np); // ok
    // operator delete[](buf); // ok
    // delete[] buf;
    // warning: cannot delete expression with pointer-to-'void'
    // type 'void *' [-Wdelete-incomplete]
    delete[] np; // undefined

    // call EP(int), 100
    // call EP(int), 200
    // 200deleted...
    // 100deleted...
}

int main(int argc, char const *argv[]) {
    // t1();
    t2();
    return 0;
}