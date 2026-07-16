// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : gaps_not_yet_fixed_in_cpp23
// Topic id : part6/b/section02/gaps_not_yet_fixed_in_cpp23
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section02/gaps_not_yet_fixed_in_cpp23";

const std::string& identity(const std::string& value) {
    return value;
}
std::string own_identity(std::string value) {
    return value;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::string owner = own_identity("owned");
    const std::string source = "borrowed";
    const std::string& safe_borrow = identity(source);
    LEARN_EXPECT_EQ(checks, owner, std::string{"owned"});
    LEARN_EXPECT_EQ(checks, safe_borrow, source);
#if 0
    const std::string& dangling = identity(std::string{"temporary"});
#endif
    // A function parameter's temporary is destroyed at the end of the caller's full-expression.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section02/gaps_not_yet_fixed_in_cpp23", run>;

}  // namespace
