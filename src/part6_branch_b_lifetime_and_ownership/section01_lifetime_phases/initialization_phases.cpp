// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : initialization_phases
// Topic id : part6/b/section01/initialization_phases
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section01/initialization_phases";

constinit int constant_initialized = 7;
int dynamic_calls{};

int dynamic_value() {
    ++dynamic_calls;
    return 9;
}

int& lazy_value() {
    static int value = dynamic_value();
    return value;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, constant_initialized, 7);
    const int before = dynamic_calls;
    int& first = lazy_value();
    int& second = lazy_value();
    LEARN_EXPECT_EQ(checks, first, 9);
    LEARN_EXPECT(checks, &first == &second);
    LEARN_EXPECT_EQ(checks, dynamic_calls, before + 1);
    // Static storage is zero-initialized first; constant initialization precedes dynamic initialization.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section01/initialization_phases", run>;

}  // namespace
