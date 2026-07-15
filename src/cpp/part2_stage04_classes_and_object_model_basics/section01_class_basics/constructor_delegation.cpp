// Topic     : 委托构造 / 初始化列表
// Doc       : 第2部分-阶段4 · 步骤 1.3
// cppreference: https://en.cppreference.com/cpp/language/constructor
//
// 要点: 成员初始化列表直接构造（优于体中赋值）；成员按【声明顺序】初始化；
//       委托构造把共性逻辑集中到一个构造函数，避免重复。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

namespace {

class Widget {
    std::string name_;
    int id_;

public:
    // ✅ 初始化列表：直接构造 name_ / id_
    Widget(std::string name, int id) : name_(std::move(name)), id_(id) {}

    // ⚠️ 成员按声明顺序初始化：先 name_ 再 id_（与列表书写顺序无关）
    // 若 id_ 写在 name_ 前面，而 id_ 初始化依赖 name_，会读到未初始化的 name_ → UB

    const std::string& name() const { return name_; }
    int id() const { return id_; }
};

class Rect {
    int w_;
    int h_;
    int area_cache_;

public:
    // 目标构造：真正做初始化
    Rect(int w, int h) : w_(w), h_(h), area_cache_(w * h) {}

    // 委托：无参 → 0×0
    Rect() : Rect(0, 0) {}

    // 委托：正方形
    explicit Rect(int side) : Rect(side, side) {}

    int w() const { return w_; }
    int h() const { return h_; }
    int area() const { return area_cache_; }
};

class BadBodyAssign {
    std::string s_;

public:
    // 演示「体中赋值」路径：string 先默认构造，再 operator= —— 两步
    // （对比初始化列表一步直接构造；教学用，生产优先初始化列表）
    explicit BadBodyAssign(std::string s) { s_ = std::move(s); }

    const std::string& get() const { return s_; }
};

// 委托构造限制：委托后，该构造的函数体里不能再初始化成员（已由被委托者完成）
class Logger {
    std::string tag_;
    int level_;

public:
    Logger(std::string tag, int level) : tag_(std::move(tag)), level_(level) {}

    explicit Logger(std::string tag) : Logger(std::move(tag), 0) {
        // 这里可以写额外逻辑（计数、校验），但不能再写 : tag_(...)
    }

    const std::string& tag() const { return tag_; }
    int level() const { return level_; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [constructor_delegation] ===\n";

    // -------------------------------------------------------------------------
    // §入门：初始化列表
    // -------------------------------------------------------------------------
    Widget w{"box", 7};
    assert(w.name() == "box" && w.id() == 7);
    std::cout << "[intro] Widget name=" << w.name() << " id=" << w.id() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：委托构造复用
    // -------------------------------------------------------------------------
    Rect r0;
    assert(r0.w() == 0 && r0.h() == 0 && r0.area() == 0);

    Rect square{5};
    assert(square.w() == 5 && square.h() == 5 && square.area() == 25);

    Rect rect{3, 4};
    assert(rect.area() == 12);
    std::cout << "[advanced] delegated Rect areas: " << r0.area() << ' ' << square.area() << ' ' << rect.area() << '\n';

    // -------------------------------------------------------------------------
    // §专家：声明顺序、体中赋值、委托后体
    // -------------------------------------------------------------------------
    BadBodyAssign b{"hi"};
    assert(b.get() == "hi");

    Logger log{"net"};
    assert(log.tag() == "net" && log.level() == 0);

    // 不能同时委托又写成员初始化列表：
    // Ctor() : Ctor(0), x_(1) {}  // ❌ 非法
    std::cout << "[expert] body-assign works but two-step; Logger tag=" << log.tag() << '\n';
    std::cout << "=== constructor_delegation: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section01/constructor_delegation", run>;

}  // namespace
