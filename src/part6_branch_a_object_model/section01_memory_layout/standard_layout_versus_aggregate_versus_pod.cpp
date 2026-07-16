// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section01_memory_layout
// Item     : standard_layout_versus_aggregate_versus_pod
// Topic id : part6/a/section01/standard_layout_versus_aggregate_versus_pod
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/a/section01/standard_layout_versus_aggregate_versus_pod";

struct Plain {
    int x;
    double y;
};

struct WithConstructor {
    explicit WithConstructor(int input) : value(input) {}
    int value;
};

struct VirtualType {
    virtual ~VirtualType() = default;
    int value{};
};

static_assert(std::is_aggregate_v<Plain>);
static_assert(std::is_standard_layout_v<Plain>);
static_assert(std::is_trivial_v<Plain>);
static_assert(!std::is_aggregate_v<WithConstructor>);
static_assert(std::is_standard_layout_v<WithConstructor>);
static_assert(!std::is_standard_layout_v<VirtualType>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const Plain value{3, 4.5};
    LEARN_EXPECT_EQ(checks, value.x, 3);
    LEARN_EXPECT_EQ(checks, WithConstructor{7}.value, 7);
    // "POD" is legacy vocabulary; state the required traits separately.
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part6/a/section01/standard_layout_versus_aggregate_versus_pod", run>;

}  // namespace
