// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section08_inheritance_polymorphism_basics
// Item     : virtual_functions
// Topic id : part2/stage04/section08/virtual_functions
//
// Covers: virtual dispatch via base reference/pointer

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <string>

namespace {

class Shape {
public:
    virtual double area() const { return 0; }
    virtual std::string name() const { return "Shape"; }
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double r_ = 0;

public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
    std::string name() const override { return "Circle"; }
};

class Square : public Shape {
    double s_ = 0;

public:
    explicit Square(double s) : s_(s) {}
    double area() const override { return s_ * s_; }
    std::string name() const override { return "Square"; }
};

void demo_basics() {
    Circle c{2.0};
    Shape& s = c;
    assert(s.name() == "Circle");
    assert(s.area() > 12.0 && s.area() < 13.0);
}

void demo_intermediate() {
    Square q{3.0};
    Shape* p = &q;
    assert(p->name() == "Square");
    assert(p->area() == 9.0);
}

void demo_expert() {
    std::unique_ptr<Shape> p = std::make_unique<Circle>(1.0);
    assert(p->name() == "Circle");
    assert(p->area() > 3.0 && p->area() < 4.0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/virtual_functions", run>;

}  // namespace
