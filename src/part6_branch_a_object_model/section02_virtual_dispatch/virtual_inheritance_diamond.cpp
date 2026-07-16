// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : virtual_inheritance_diamond
// Topic id : part6/a/section02/virtual_inheritance_diamond
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/a/section02/virtual_inheritance_diamond";

struct Root {
    virtual ~Root() = default;
    int value{7};
};
struct Left : virtual Root {};
struct Right : virtual Root {};
struct Diamond : Left, Right {};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Diamond object;
    Root* through_left = static_cast<Left*>(&object);
    Root* through_right = static_cast<Right*>(&object);
    LEARN_EXPECT(checks, through_left == through_right);  // One shared virtual-base subobject.
    through_left->value = 23;
    LEARN_EXPECT_EQ(checks, through_right->value, 23);
    LEARN_EXPECT(checks, dynamic_cast<Diamond*>(through_left) == &object);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section02/virtual_inheritance_diamond", run>;

}  // namespace
