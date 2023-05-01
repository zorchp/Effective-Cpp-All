#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <typeinfo>
using namespace std;

// 根据多个参数虚化某一基类虚函数

namespace version1 {

class GameObject {
public:
    virtual void collide(GameObject& otherObject) = 0;
};

class SpaceShip : public GameObject {
public:
    virtual void collide(GameObject& otherObject);
};

class SpaceStation : public GameObject {
public:
    virtual void collide(GameObject& otherObject) {}
};

class Asteroid : public GameObject {
public:
    virtual void collide(GameObject& otherObject) {}
};

class CollisionWithUnknoenObject {
public:
    CollisionWithUnknoenObject(GameObject& whatWeHit) {}
};

void SpaceShip::collide(GameObject& otherObject) {
    // version1: 冗杂的代码, 通过分支实现
    const type_info& otherType = typeid(otherObject);

    if (otherType == typeid(SpaceShip)) {
        /* SpaceShip& ss = static_cast<SpaceShip&>(otherObject); */
        cout << "this is SpaceShip\n";
    } else if (otherType == typeid(SpaceStation)) {
        /* SpaceStation& ss = static_cast<SpaceStation&>(otherObject); */
        cout << "this is SpaceStation\n";
    } else if (otherType == typeid(Asteroid)) {
        /* Asteroid& a = static_cast<Asteroid&>(otherObject); */
        cout << "this is Asteroid\n";
    } else {
        throw CollisionWithUnknoenObject(otherObject);
    }
}

} // namespace version1

namespace version2 {

class SpaceShip;
class SpaceStation;
class Asteroid;

class GameObject {
public:
    virtual void collide(GameObject& otherObject) = 0;
    virtual void collide(SpaceShip& otherObject) = 0;
    virtual void collide(SpaceStation& otherObject) = 0;
    virtual void collide(Asteroid& otherObject) = 0;
};

class SpaceShip : public GameObject {
public:
    // 重载collide, 每一个版本对应继承体系中的一个派生类
    virtual void collide(GameObject& otherObject) {
        // 并非递归调用, this所指为SpaceShip
        cout << "call SpaceShip::collide(GameObject&)\n";
        otherObject.collide(*this);
    }
    virtual void collide(SpaceShip& otherObject) {
        cout << "process SpaceShip collision\n";
    }
    virtual void collide(SpaceStation& otherObject) {
        cout << "process SpaceStation collision\n";
    }
    virtual void collide(Asteroid& otherObject) {
        cout << "process Asteroid collision\n";
    }
};

class SpaceStation : public GameObject {
public:
    virtual void collide(GameObject& otherObject) {
        // 并非递归调用, this所指为SpaceStation
        cout << "call SpaceStation::collide(GameObject&)\n";
        otherObject.collide(*this);
    }
    virtual void collide(SpaceShip& otherObject) {
        cout << "process SpaceShip collision\n";
    }
    virtual void collide(SpaceStation& otherObject) {
        cout << "process SpaceStation collision\n";
    }
    virtual void collide(Asteroid& otherObject) {
        cout << "process Asteroid collision\n";
    }
};

class Asteroid : public GameObject {
public:
    virtual void collide(GameObject& otherObject) {
        // 并非递归调用, this所指为Asteroid
        cout << "call Asteroid::collide(GameObject&)\n";
        otherObject.collide(*this);
    }
    virtual void collide(SpaceShip& otherObject) {
        cout << "process SpaceShip collision\n";
    }
    virtual void collide(SpaceStation& otherObject) {
        cout << "process SpaceStation collision\n";
    }
    virtual void collide(Asteroid& otherObject) {
        cout << "process Asteroid collision\n";
    }
};

} // namespace version2


void t1() {
    // 不用if-else实现, 效率提高
    // 可以用但是新加入类之后就要重写代码, 并不好用
    using namespace version2;
    SpaceShip s1;
    SpaceStation st;
    Asteroid as;

    s1.collide(s1); // process SpaceShip collision
    s1.collide(st); // process SpaceStation collision
    s1.collide(as); // process Asteroid collision

    // 动态多态
    GameObject* g1 = new SpaceShip();
    s1.collide(*g1);
    /* call SpaceShip::collide(GameObject&) */
    /* process SpaceShip collision */
}

namespace version3 {
// 仿真虚函数表

class SpaceShip;
class SpaceStation {};
class Asteroid {};

class GameObject {
public:
    virtual void collide(GameObject& otherObject) = 0;
};

class CollisionWithUnknoenObject {
public:
    CollisionWithUnknoenObject(GameObject& whatWeHit) {}
};


class SpaceShip : public GameObject {
public:
    virtual void collide(GameObject& otherObject) {
        HitFunctionPtr hfp = lookup(otherObject);
        if (hfp)
            (this->*hfp)(otherObject);
        else
            // 仍然要注意异常情况
            throw CollisionWithUnknoenObject(otherObject);
    }

    // 放弃了重载, 以便解耦
    virtual void hitSpaceShip(GameObject& spaceship) {
        // 其他类似也要转型, 以保证预期行为的真实性
        /* SpaceShip& otherShip = dynamic_cast<SpaceShip&>(spaceship); */
        cout << "process SpaceShip collision\n";
    }
    virtual void hitSpaceStation(GameObject& spacestation) {
        cout << "process SpaceStation collision\n";
    }
    virtual void hitAsteroid(GameObject& otherObject) {
        cout << "process Asteroid collision\n";
    }

private:
    // 函数指针声明
    typedef void (SpaceShip::*HitFunctionPtr)(GameObject&);
    typedef map<string, HitFunctionPtr> HitMap;

    static HitFunctionPtr lookup(const GameObject& whatWeHit) {
        static unique_ptr<HitMap> collisionMap(initializeCollisionMap());
        HitMap::iterator mapEntry =
            collisionMap->find(typeid(whatWeHit).name());
        if (mapEntry == collisionMap->end()) return 0;
        /* return (*mapEntry).second; */
        return mapEntry->second;
    }

    static HitMap* initializeCollisionMap() {
        // init map
        HitMap* phm = new HitMap;
        // 这里需要指定类作用域
        (*phm)["SpaceShip"] = &SpaceShip::hitSpaceShip;
        (*phm)["SpaceStation"] = &SpaceShip::hitSpaceStation;
        (*phm)["Asteroid"] = &SpaceShip::hitAsteroid;
        return phm;
    }
};
} // namespace version3

void t2() {
    using namespace version3;
    //
    SpaceShip s1;

    /* s1.collide(s1); // process SpaceShip collision */

    // 动态多态
    /* GameObject* g1 = new SpaceShip(); */
    /* s1.collide(*g1); */
}

namespace version4 {

class SpaceShip;
class SpaceStation {};
class Asteroid {};

class GameObject {
public:
    virtual void collide(GameObject& otherObject) = 0;
};

class UnknownCollision {
public:
    UnknownCollision(GameObject& whatWeHit, GameObject& toHit) {}
};

namespace { // 匿名命名空间, 作用相当于static
// 主要的碰撞处理函数
void shipAsteroid(GameObject& spaceShip, GameObject& asteroid) {
    cout << "call shipAsteroid\n";
}
void shipStation(GameObject& spaceShip, GameObject& spaceStation) {
    cout << "call shipStation\n";
}
void asteroidStation(GameObject& asteroid, GameObject& shipStation) {
    cout << "call asteroidStation\n";
}
// 次要的碰撞处理函数, 对调参数位置, 实现对称性, 通过调用主要碰撞处理函数来实现
void asteroidShip(GameObject& asteroid, GameObject& spaceShip) {
    shipAsteroid(spaceShip, asteroid);
}
void stationShip(GameObject& spaceStation, GameObject& spaceShip) {
    shipStation(spaceShip, spaceStation);
}
void stationAsteroid(GameObject& spaceStation, GameObject& asteroid) {
    shipAsteroid(asteroid, spaceStation);
}

typedef void (*HitFunctionPtr)(GameObject&, GameObject&);
typedef map<pair<string, string>, HitFunctionPtr> HitMap;
pair<string, string> makeStringPair(const char* s1, const char* s2) {
    return pair<string, string>(s1, s2);
}

HitMap* initializeCollisionMap() {
    HitMap* phm = new HitMap;
    (*phm)[makeStringPair("SpaceShip", "Asteroid")] = &shipAsteroid;
    (*phm)[makeStringPair("SpaceShip", "SpaceStation")] = &shipStation;
    return phm;
}
HitFunctionPtr lookup(const string& class1, const string& class2) {
    static unique_ptr<HitMap> collisionMap(initializeCollisionMap());
    HitMap::iterator mapEntry = collisionMap->find(make_pair(class1, class2));
    if (mapEntry == collisionMap->end()) return 0;
    return mapEntry->second;
}

} // namespace

void peocessCollision(GameObject& object1, GameObject& object2) {
    HitFunctionPtr phf = lookup(typeid(object1).name(), typeid(object2).name());
    if (phf)
        phf(object1, object2);
    else
        throw UnknownCollision(object1, object2);
}

} // namespace version4

namespace version5 {
// 可以直接加入map, 不需要修改内置类型

class SpaceShip;
class SpaceStation {};
class Asteroid {};

class GameObject {
public:
    virtual void collide(GameObject& otherObject) = 0;
};

class CollisionWithUnknoenObject {
public:
    CollisionWithUnknoenObject(GameObject& whatWeHit) {}
};


class CollisionMap {
public:
    typedef void (*HitFunctionPtr)(GameObject&, GameObject&);
    void addEntry(const string& type1, const string& type2,
                  HitFunctionPtr CollisionFunction, bool symmetic = true);
    void removeEntry(const string& type1, const string& type2,
                     HitFunctionPtr CollisionFunction, bool symmetic = true);
    HitFunctionPtr lookup(const string& type1, const string& type2);

    // 返回引用, 表示仅有一个map, 单例
    static CollisionMap& theCollisionMap();

private:
    // 私有, 防止产生多个map
    CollisionMap();
    CollisionMap(const CollisionMap&);
};
// 需要确保map条目再起对应的任何撞击发生之前就被加入map中, 方法是进行检查
// 可以创建一个类
class RegisterCollisionFunction {
public:
    RegisterCollisionFunction(const string& type1, const string& type2,
                              CollisionMap::HitFunctionPtr collisionFunction,
                              bool symmetic = true) {
        CollisionMap::theCollisionMap().addEntry(type1, type2,
                                                 collisionFunction, symmetic);
    }
};

// 如果创建一个子类:
class Satellite : public GameObject {};

void satelliteShip(GameObject& satellite, GameObject& spaceship);

// 只需要这样注册即可, 不需要修改原始类的代码
RegisterCollisionFunction cf4("Satellite", "SpaceShip", &satelliteShip);


} // namespace version5

void t3() {
    //
    using namespace version5;
    // test:
    void shipAsteroid(GameObject & spaceShip, GameObject & asteroid);
    CollisionMap::theCollisionMap().addEntry("SpaceShip", "Asteroid",
                                             shipAsteroid);
    // 使用注册类来创建需要的碰撞处理函数
    RegisterCollisionFunction cf1("SpaceShip", "Asteroid", &shipAsteroid);
}

int main(int argc, char* argv[]) {
    /* t1(); */
    /* t2(); */
    t3();
    return 0;
}
