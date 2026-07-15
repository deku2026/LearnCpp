// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section08_inheritance_polymorphism_basics
// Item     : final_specifier
// Topic id : part2/stage04/section08/final_specifier
//
// Covers: final on virtual functions and final classes

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

class Base {
public:
    virtual int f() { return 1; }
    virtual int g() { return 10; }
    virtual ~Base() = default;
};

class Mid : public Base {
public:
    int f() override { return 2; }
    int g() final { return 20; }
};

class Leaf final : public Mid {
public:
    int f() override { return 3; }
    // int g() override; // would fail: g is final
};

void demo_basics() {
    Leaf leaf;
    assert(leaf.f() == 3);
    assert(leaf.g() == 20);
}

void demo_intermediate() {
    Leaf leaf;
    Base& b = leaf;
    assert(b.f() == 3);
    assert(b.g() == 20);
}

void demo_expert() {
    static_assert(std::is_final_v<Leaf>);
    Mid m;
    Base& b = m;
    assert(b.g() == 20);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/final_specifier", run>;

}  // namespace
