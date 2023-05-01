#include "Widget.h"
#include <memory>
#include <type_traits>

struct Widget::Impl {
    string name;
    vector<double> data;
    Gadget g1, g2, g3;
};

Widget::Widget() : pImpl(make_unique<Impl>()) {}
// 默认均在定义处加
Widget::~Widget() = default;

Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(Widget&& rhs) = default;

Widget::Widget(const Widget& rhs) : pImpl(make_unique<Impl>(*rhs.pImpl)) {}
Widget& Widget::operator=(const Widget& rhs) {
    *pImpl = *rhs.pImpl;
    return *this;
}

Widget1::Widget1() : pImpl(make_shared<Impl>()) {}
