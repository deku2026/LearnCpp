// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section08_inheritance_polymorphism_basics
// Item     : override_specifier
// Topic id : part2/stage04/section08/override_specifier
//
// Covers: override catches signature mistakes; always mark overrides

#include "learn/topic_registry.hpp"

#include <string>

namespace {

class Base {
public:
    virtual int f() const { return 1; }
    virtual std::string g() { return "base"; }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    int f() const override { return 2; }
    std::string g() override { return "derived"; }
    // int f() override; // would fail: missing const
};

void demo_basics() {
    Derived d;
    LEARN_CHECK(d.f() == 2);
    LEARN_CHECK(d.g() == "derived");
}

void demo_intermediate() {
    Derived d;
    Base& b = d;
    LEARN_CHECK(b.f() == 2);
    LEARN_CHECK(b.g() == "derived");
}

void demo_expert() {
    // Without override, a mismatched signature silently creates a new function.
    // With override, the compiler rejects the mismatch.
    Derived d;
    const Base& b = d;
    LEARN_CHECK(b.f() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/override_specifier", run>;

}  // namespace
