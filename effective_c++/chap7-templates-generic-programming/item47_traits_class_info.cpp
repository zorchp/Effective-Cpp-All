#include <iostream>
#include <iterator>
#include <exception>
#include <list>
#include <vector>
#include <set>
#include <unordered_set>


using namespace std;

template <typename IterT, typename DistT>
void doAdvanced(IterT& iter, DistT d, random_access_iterator_tag) {
    iter += d;
    cout << "call random_access_iterator_tag\n";
}

template <typename IterT, typename DistT>
void doAdvanced(IterT& iter, DistT d, bidirectional_iterator_tag) {
    if (d >= 0) {
        while (d--) ++iter;
    } else {
        while (d++) --iter;
    }
    cout << "call bidirectional_iterator_tag\n";
}

template <typename IterT, typename DistT>
void doAdvanced(IterT& iter, DistT d, input_iterator_tag) {
    if (d < 0) throw out_of_range("nagative distance");
    while (d--) ++iter;
    cout << "call input_iterator_tag\n";
}

template <typename IterT, typename DistT>
void Advanced(IterT& iter, DistT d) {
    doAdvanced(iter, d, typename iterator_traits<IterT>::iterator_category());
}

void t1() {
    list<int> l{1, 2, 3, 4, 5};
    auto it = l.begin();
    Advanced(it, 3);     // call bidirectional_iterator_tag
    cout << *it << endl; // 4
}

void t2() {
    vector<int> l{1, 2, 3, 4, 5};
    auto it = l.begin();
    Advanced(it, 3);     // call random_access_iterator_tag
    cout << *it << endl; // 4
}

void t3() {
    unordered_set<int> l{1, 2, 3, 4, 5};
    auto it = l.begin();
    Advanced(it, 3);     // call input_iterator_tag
    cout << *it << endl; // 4
}

int main(int argc, char* argv[]) {
    /* t1(); */
    /* t2(); */
    t3();
    return 0;
}
