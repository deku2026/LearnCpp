// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : object_slicing
// Topic id : part6/a/section02/object_slicing
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <memory>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part6/a/section02/object_slicing";

struct Base {
    virtual ~Base() = default;
    virtual std::string_view kind() const { return "base"; }
};

struct Derived : Base {
    std::string_view kind() const override { return "derived"; }
    int detail{9};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const Derived derived;
    const Base sliced = derived;
    LEARN_EXPECT_EQ(checks, sliced.kind(), std::string_view{"base"});
    std::vector<std::unique_ptr<Base>> polymorphic;
    polymorphic.push_back(std::make_unique<Derived>());
    LEARN_EXPECT_EQ(checks, polymorphic.front()->kind(), std::string_view{"derived"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section02/object_slicing", run>;

}  // namespace
