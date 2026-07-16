// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_span
// Topic id : part6/b/section03/dangling_span
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <span>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section03/dangling_span";

int sum(std::span<const int> values) {
    int result{};
    for (const int value : values) result += value;
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::array owner{1, 2, 3};
    const std::span view{owner};
    LEARN_EXPECT_EQ(checks, sum(view), 6);
    LEARN_EXPECT(checks, view.data() == owner.data());
#if 0
    std::span<const int> dangling;
    { std::array local{1, 2}; dangling = local; }
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section03/dangling_span", run>;

}  // namespace
