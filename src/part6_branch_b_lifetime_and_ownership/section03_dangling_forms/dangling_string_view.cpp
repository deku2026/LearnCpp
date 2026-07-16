// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_string_view
// Topic id : part6/b/section03/dangling_string_view
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section03/dangling_string_view";

std::string_view prefix(std::string_view text) {
    return text.substr(0, 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::string owner = "abcdef";
    const std::string_view view = prefix(owner);
    LEARN_EXPECT_EQ(checks, view, std::string_view{"abc"});
    LEARN_EXPECT(checks, view.data() == owner.data());
#if 0
    std::string_view dangling = prefix(std::string{"temporary"});
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section03/dangling_string_view", run>;

}  // namespace
