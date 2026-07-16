// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : msvc_core_check
// Topic id : part6/b/section06/msvc_core_check
// References: C++23 [basic.life], [meta.unary.prop], C++ Core Guidelines lifetime profile

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <ranges>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section06/msvc_core_check";

struct Checkset {
    std::string_view name;
    bool lifetime_related;
};
constexpr std::array checks_list{
    Checkset{"CppCoreCheck", true},
    Checkset{"NativeRecommendedRules", true},
    Checkset{"Security Development Lifecycle checks", false},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto lifetime_count = std::ranges::count_if(checks_list, &Checkset::lifetime_related);
    LEARN_EXPECT_EQ(checks, lifetime_count, 2);
    LEARN_EXPECT_EQ(checks, checks_list.size(), 3U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section06/msvc_core_check", run>;

}  // namespace
