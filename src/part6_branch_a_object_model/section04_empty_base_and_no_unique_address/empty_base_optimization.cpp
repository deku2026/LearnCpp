// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section04_empty_base_and_no_unique_address
// Item     : empty_base_optimization
// Topic id : part6/a/section04/empty_base_optimization
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/a/section04/empty_base_optimization";

struct EmptyPolicy {};
struct Member {
    EmptyPolicy policy;
    int value;
};
struct Base : EmptyPolicy {
    int value;
};

static_assert(std::is_empty_v<EmptyPolicy>);
static_assert(sizeof(Base) <= sizeof(Member));

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Base value{{}, 7};
    LEARN_EXPECT_EQ(checks, value.value, 7);
    LEARN_EXPECT(checks, sizeof(EmptyPolicy) >= 1U);  // Complete objects still need distinct addresses.
    LEARN_EXPECT(checks, sizeof(Base) >= sizeof(int));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section04/empty_base_optimization", run>;

}  // namespace
