// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_reference
// Topic id : part6/b/section03/dangling_reference
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <functional>
#include <ranges>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section03/dangling_reference";

std::reference_wrapper<const std::string> borrow(const std::string& value) {
    return value;
}
std::string make_value() {
    return "owned";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::string owner = make_value();
    const auto reference = borrow(owner);
    LEARN_EXPECT_EQ(checks, reference.get(), std::string{"owned"});
#if 0
    const std::string& dangling = make_value();
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section03/dangling_reference", run>;

}  // namespace
