// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : core_guidelines_lifetime_profile
// Topic id : part6/b/section06/core_guidelines_lifetime_profile
// References: C++23 [basic.life], [meta.unary.prop], C++ Core Guidelines lifetime profile

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <ranges>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section06/core_guidelines_lifetime_profile";

struct Guideline {
    std::string_view id;
    std::string_view practice;
};
constexpr std::array guidelines{
    Guideline{"R.3", "raw pointer denotes a non-owning borrow"},
    Guideline{"R.5", "prefer scoped resource objects"},
    Guideline{"F.43", "never return a reference to a local"},
    Guideline{"F.53", "avoid reference captures that outlive the scope"},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, guidelines.size(), 4U);
    LEARN_EXPECT(checks, guidelines[0].practice.contains("non-owning"));
    LEARN_EXPECT(checks, guidelines[2].practice.contains("never"));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section06/core_guidelines_lifetime_profile", run>;

}  // namespace
