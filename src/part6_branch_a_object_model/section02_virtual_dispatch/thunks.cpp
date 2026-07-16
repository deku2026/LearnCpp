// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : thunks
// Topic id : part6/a/section02/thunks
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/a/section02/thunks";

struct Left {
    virtual ~Left() = default;
    virtual const void* self_from_left() const = 0;
};
struct Right {
    virtual ~Right() = default;
    virtual const void* self_from_right() const = 0;
};

struct Combined final : Left, Right {
    const void* self_from_left() const override { return this; }
    const void* self_from_right() const override { return this; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Combined object;
    Left* left = &object;
    Right* right = &object;
    LEARN_EXPECT(checks, left->self_from_left() == &object);
    LEARN_EXPECT(checks, right->self_from_right() == &object);
    // An ABI may use an adjustment thunk so a call through Right receives Combined's this pointer.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section02/thunks", run>;

}  // namespace
