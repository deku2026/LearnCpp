// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : clang_tidy_lifetime_rules
// Topic id : part6/b/section06/clang_tidy_lifetime_rules
// References: C++23 [basic.life], [meta.unary.prop], C++ Core Guidelines lifetime profile

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <ranges>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section06/clang_tidy_lifetime_rules";

struct Rule {
    std::string_view name;
    std::string_view concern;
};
constexpr std::array rules{
    Rule{"bugprone-dangling-handle", "views outliving owners"},
    Rule{"cppcoreguidelines-owning-memory", "ambiguous raw ownership"},
    Rule{"clang-analyzer-cplusplus.NewDelete", "allocation/deallocation paths"},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, rules.size(), 3U);
    LEARN_EXPECT(checks, std::ranges::all_of(rules, [](const Rule& rule) { return !rule.concern.empty(); }));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section06/clang_tidy_lifetime_rules", run>;

}  // namespace
