// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_iterator
// Topic id : part6/b/section03/dangling_iterator
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part6/b/section03/dangling_iterator";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values{1, 2, 3};
    const std::size_t index = 1;
    values.reserve(32);  // Any old iterator would be invalid if allocation changed.
    auto reacquired = values.begin() + static_cast<std::ptrdiff_t>(index);
    LEARN_EXPECT_EQ(checks, *reacquired, 2);
    reacquired = values.erase(reacquired);
    LEARN_EXPECT_EQ(checks, *reacquired, 3);
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({1, 3}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section03/dangling_iterator", run>;

}  // namespace
