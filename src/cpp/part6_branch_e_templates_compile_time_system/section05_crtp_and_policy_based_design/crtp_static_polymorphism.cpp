// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E5 CRTP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section05_crtp_and_policy_based_design
// Item     : crtp_static_polymorphism
// Topic id : part6/e/section05/crtp_static_polymorphism
//
// 要点: CRTP = 基类模板参数是派生类 → 静态多态, 无 vptr。
// 参考: [temp.mem] C++ Templates Complete Guide; CRTP idiom

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {

// 基类提供统一算法, 派生类提供钩子
template <typename Derived>
struct Shape {
    double area() const { return static_cast<const Derived*>(this)->area_impl(); }
    std::string name() const { return static_cast<const Derived*>(this)->name_impl(); }
};

struct Circle : Shape<Circle> {
    double r;
    explicit Circle(double radius) : r(radius) {}
    double area_impl() const { return 3.141592653589793 * r * r; }
    std::string name_impl() const { return "circle"; }
};

struct Square : Shape<Square> {
    double side;
    explicit Square(double s) : side(s) {}
    double area_impl() const { return side * side; }
    std::string name_impl() const { return "square"; }
};

// 注入相等比较
template <typename D>
struct EqualityComparable {
    friend bool operator==(const D& a, const D& b) { return a.equal_to(b); }
    friend bool operator!=(const D& a, const D& b) { return !(a == b); }
};

struct Point : EqualityComparable<Point> {
    int x = 0;
    int y = 0;
    Point() = default;
    Point(int x_, int y_) : x(x_), y(y_) {}
    bool equal_to(const Point& o) const { return x == o.x && y == o.y; }
};

// 计数 mixin
template <typename D>
struct InstanceCounter {
    static int live;
    InstanceCounter() { ++live; }
    InstanceCounter(const InstanceCounter&) { ++live; }
    ~InstanceCounter() { --live; }
};
template <typename D>
int InstanceCounter<D>::live = 0;

struct Widget : InstanceCounter<Widget> {
    int id = 0;
};

// 统一接口处理 (编译期, 非异构容器)
template <typename S>
double twice_area(const Shape<S>& s) {
    return 2.0 * s.area();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E5 CRTP static polymorphism ===\n";

    Circle c(2.0);
    Square q(3.0);
    assert(c.name() == "circle");
    assert(q.name() == "square");
    assert(c.area() > 12.5 && c.area() < 12.6);
    assert(q.area() == 9.0);
    assert(twice_area(c) > 25.0);

    Point a{1, 2};
    Point b{1, 2};
    Point d{0, 0};
    assert(a == b);
    assert(a != d);

    {
        Widget w1, w2;
        assert(InstanceCounter<Widget>::live == 2);
    }
    assert(InstanceCounter<Widget>::live == 0);

    std::cout << "  no vptr; each Shape<Derived> is a distinct type\n";
    std::cout << "  cannot put Circle/Square in one vector<ShapeBase*> without erasure\n";
    std::cout << "crtp_static_polymorphism: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section05/crtp_static_polymorphism", run>;

}  // namespace
