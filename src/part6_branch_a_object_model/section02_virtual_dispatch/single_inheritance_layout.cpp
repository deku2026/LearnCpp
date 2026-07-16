// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : single_inheritance_layout
// Topic id : part6/a/section02/single_inheritance_layout
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <cstdint>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/a/section02/single_inheritance_layout";

struct Base {
    virtual ~Base() = default;
    virtual int score() const { return base_value; }
    int base_value{3};
};

struct Derived : Base {
    int score() const override { return base_value + extra; }
    int extra{4};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Derived derived;
    Base* base = &derived;
    LEARN_EXPECT_EQ(checks, base->score(), 7);
    LEARN_EXPECT(checks, dynamic_cast<Derived*>(base) == &derived);
    LEARN_EXPECT(checks, sizeof(Derived) >= sizeof(Base));
    LEARN_EXPECT_EQ(checks, derived.extra, 4);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section02/single_inheritance_layout", run>;

}  // namespace
