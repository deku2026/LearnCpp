// Topic     : 纯虚函数与抽象类
// Doc       : 第2部分-阶段4 · 步骤 9.3
// cppreference: https://en.cppreference.com/cpp/language/abstract_class
//
// 要点: =0 使类抽象、不可实例化；派生必须覆盖后才能实例化；纯虚也可有定义。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace {

class AbstractShape {
public:
    virtual ~AbstractShape() = default;
    virtual double area() const = 0;
    virtual std::string name() const = 0;

    // 纯虚仍可提供默认实现（必须类外定义）
    virtual void tag() const = 0;
};

void AbstractShape::tag() const {
    // 默认实现：派生可用 AbstractShape::tag() 限定调用
}

class Circle : public AbstractShape {
    double r_;

public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
    std::string name() const override { return "Circle"; }
    void tag() const override { AbstractShape::tag(); }
};

class Rect : public AbstractShape {
    double w_, h_;

public:
    Rect(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    std::string name() const override { return "Rect"; }
    void tag() const override {}
};

// 仍抽象：未覆盖全部纯虚
class Almost : public AbstractShape {
public:
    double area() const override { return 1.0; }
    // name/tag 仍纯虚 → Almost 仍抽象
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pure_virtual_and_abstract] ===\n";

    // -------------------------------------------------------------------------
    // §入门：不能实例化抽象类
    // -------------------------------------------------------------------------
    // AbstractShape s; // ❌
    Circle c{1.0};
    assert(c.name() == "Circle");
    std::cout << "[intro] Circle area~" << c.area() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：抽象接口容器
    // -------------------------------------------------------------------------
    std::vector<std::unique_ptr<AbstractShape>> v;
    v.push_back(std::make_unique<Circle>(2.0));
    v.push_back(std::make_unique<Rect>(2.0, 3.0));
    double sum = 0;
    for (const auto& s : v) {
        sum += s->area();
        s->tag();
    }
    assert(sum > 12.0);
    std::cout << "[advanced] interface sum~" << sum << '\n';

    // -------------------------------------------------------------------------
    // §专家：部分覆盖仍抽象；纯虚定义
    // -------------------------------------------------------------------------
    // Almost a; // ❌
    static_assert(std::is_abstract_v<AbstractShape>);
    static_assert(std::is_abstract_v<Almost>);
    static_assert(!std::is_abstract_v<Circle>);

    AbstractShape* p = &c;
    p->tag();  // 动态调用 Circle::tag → 可再调到基类定义

    std::cout << "[expert] is_abstract checks ok\n";
    std::cout << "=== pure_virtual_and_abstract: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/pure_virtual_and_abstract", run>;

}  // namespace
