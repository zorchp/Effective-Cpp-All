#include <iostream>
using namespace std;

class Shape {
public:
    virtual void draw() const = 0;
    virtual void error(const string& msg);
    int objID() const;
};

void Shape::draw() const { cout << "Shape::draw()\n "; }

class Rectangle : public Shape {
public:
    void draw() const { cout << "Rectangle::draw()\n"; }
};
class Ellipse : public Shape {
public:
    void draw() const { cout << "Ellipse::draw()\n"; }
};


void t1() {
    /* Shape*ps = new Shape; */
    Shape* ps1 = new Rectangle;
    ps1->draw();
    Shape* ps2 = new Ellipse;
    ps2->draw();
    ps1->Shape::draw();
    ps2->Shape::draw();
}


int main(int argc, char* argv[]) {
    t1();
    return 0;
}
