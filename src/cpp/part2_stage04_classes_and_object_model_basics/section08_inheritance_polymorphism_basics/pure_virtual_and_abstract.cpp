// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section08_inheritance_polymorphism_basics
// Item     : pure_virtual_and_abstract
// Topic id : part2/stage04/section08/pure_virtual_and_abstract
//
// Covers: pure virtual, abstract classes, concrete overrides

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <type_traits>

namespace {

class AbstractShape {
public:
    virtual double area() const = 0;
    virtual std::string name() const = 0;
    virtual ~AbstractShape() = default;
};

class Circle : public AbstractShape {
    double r_ = 0;

public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
    std::string name() const override { return "Circle"; }
};

void demo_basics() {
    Circle c{1.0};
    assert(c.name() == "Circle");
    assert(c.area() > 3.0);
}

void demo_intermediate() {
    static_assert(std::is_abstract_v<AbstractShape>);
    static_assert(!std::is_abstract_v<Circle>);
    std::unique_ptr<AbstractShape> p = std::make_unique<Circle>(2.0);
    assert(p->name() == "Circle");
}

void demo_expert() {
    // Abstract types cannot be instantiated; only pointers/references to them.
    const AbstractShape& s = Circle{3.0};
    assert(s.area() > 28.0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/pure_virtual_and_abstract", run>;

}  // namespace
