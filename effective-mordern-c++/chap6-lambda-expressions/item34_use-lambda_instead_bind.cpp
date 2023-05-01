#include <chrono>
#include <iostream>

using namespace std;
// lambda 相较于bind具有更好的可读性

using Time = std::chrono::steady_clock::time_point;
enum class Sound { Beep, Siren, Whistle };
using Duration = std::chrono::steady_clock::duration;
// t时刻发声音s持续d
void setAlarm(Time t, Sound s, Duration d) {}

auto setSoundL = [](Sound s) {
    using namespace std::chrono;
    setAlarm(steady_clock::now() + hours(1), s, seconds(30));
};

auto setSoundL_14 = [](Sound s) {
    using namespace std::chrono;
    using namespace std::literals;
    setAlarm(steady_clock::now() + 1h, s, 30s);
};
// 使用bind:
using namespace std::placeholders;
using namespace std::chrono;
using namespace std::literals;
// 绑定之后才开始计算时间, 而不是调用setAlarm时候就开始计算时间了
auto setSoundB = std::bind(setAlarm, steady_clock::now() + 1h, _1, 30s);

// C++11
auto setSoundB_modify = std::bind(
    setAlarm,
    std::bind(std::plus<steady_clock::time_point>(), steady_clock::now(), 1h),
    _1, 30s);

// C++14
auto setSoundB_modify_14 = std::bind(
    setAlarm, std::bind(std::plus<>(), steady_clock::now(), 1h), _1, 30s);

void t1() {
    //
    Sound s{};
    setSoundL(s);
    setSoundB_modify_14(s);
}

// 此时进行了重载:
enum class Volume { Normal, Loud, LoudPlusPlus };

void setAlarm(Time t, Sound s, Duration d, Volume v) {}
// lambda 不变, 但是bind需要变了
/* auto setSoundB_modify_overload = std::bind( */
/*     setAlarm, std::bind(std::plus<>(), steady_clock::now(), 1h), _1, 30s); */
using SetAlarmParamType = void (*)(Time, Sound, Duration);
auto setSoundB_modify_overload =
    std::bind(static_cast<SetAlarmParamType>(setAlarm), // 需要指定函数类型
              std::bind(std::plus<>(), steady_clock::now(), 1h), _1, 30s);

void t2() {
    //
    Sound s{};
    setSoundB_modify_14(s);
    setSoundB_modify_overload(s);
}

void t3() {
    int low = 1, high = 10;
    auto betweenL = [low, high](const auto &val) { // lambda auto :C++14
        return low <= val && val <= high;
    };
    using namespace placeholders;
    auto betweenB = bind(logical_and<bool>(), bind(less_equal<int>(), low, _1),
                         bind(less_equal<int>(), _1, high));

    auto betweenB_14 = bind(logical_and<>(), bind(less_equal<>(), low, _1),
                            bind(less_equal<>(), _1, high));
}

class Widget {};
enum class CompLevel { Low, Normal, High };
Widget compress(const Widget &w, CompLevel lev) { return Widget(); }

void t4() {
    Widget w;
    auto compressRateB =
        bind(compress, w, _1); // 虽然是按值传递, 但是表面看不出来
    auto compressRateB_ref =
        bind(compress, std::ref(w), _1); // 转换为按引用传递
    auto compressRateL = [w](CompLevel lev) {
        return compress(w, lev);
    }; // 传递情况显而易见
}

// C++11 中尚可能用到的bind  使用场景
//  移动捕获: 前面已提到
//  多态函数对象
class PolyWidget {
public:
    template <typename T>
    void operator()(const T &param);
};

void t5() {
    //
    PolyWidget pw;
    auto boundPw = bind(pw, _1);
    boundPw(1);
    boundPw(nullptr);
    boundPw("hello");
    // C++14 lambda + auto 可以实现:
    auto boundPw_lambda = [pw](const auto &param) { pw(param); };
}


int main(int argc, char *argv[]) {
    /* t1(); */
    t4();
    return 0;
}
