// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : range_for_temporary_extension_cpp23
// Topic id : part6/b/section02/range_for_temporary_extension_cpp23
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part6/b/section02/range_for_temporary_extension_cpp23";

std::vector<std::string> words() {
    return {"safe", "range"};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::string observed;
#if defined(__cpp_range_based_for) && __cpp_range_based_for >= 202211L
    for (const char value : words().front()) observed.push_back(value);
#else
    auto owner = words();
    for (const char value : owner.front()) observed.push_back(value);
#endif
    LEARN_EXPECT_EQ(checks, observed, std::string{"safe"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section02/range_for_temporary_extension_cpp23", run>;

}  // namespace
