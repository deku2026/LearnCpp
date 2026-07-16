// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : vtable_vptr
// Topic id : part6/a/section02/vtable_vptr
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <memory>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/a/section02/vtable_vptr";

struct Shape {
    virtual ~Shape() = default;
    virtual std::string_view name() const = 0;
    int color{1};
};

struct Circle final : Shape {
    std::string_view name() const override { return "circle"; }
    int radius{2};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::unique_ptr<Shape> shape = std::make_unique<Circle>();
    LEARN_EXPECT_EQ(checks, shape->name(), std::string_view{"circle"});
    LEARN_EXPECT_EQ(checks, shape->color, 1);
    LEARN_EXPECT(checks, sizeof(Circle) >= sizeof(Shape));
    // Dynamic dispatch is standard; a vtable/vptr layout is a common ABI technique, not a language guarantee.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section02/vtable_vptr", run>;

}  // namespace
