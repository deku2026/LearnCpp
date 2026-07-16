// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : cppcheck_setup
// Topic id : part4/section03/cppcheck_setup
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section03/cppcheck_setup";

struct Finding {
    std::string id;
    bool inconclusive;
};

std::size_t actionable(const std::vector<Finding>& findings, const std::set<std::string>& suppressions) {
    return static_cast<std::size_t>(std::ranges::count_if(
        findings, [&](const Finding& finding) { return !finding.inconclusive && !suppressions.contains(finding.id); }));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<Finding> findings{{"nullPointer", false}, {"unusedFunction", true}, {"knownThirdParty", false}};
    LEARN_EXPECT_EQ(checks, actionable(findings, {"knownThirdParty"}), 1U);
    LEARN_EXPECT_EQ(checks, actionable(findings, {}), 2U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section03/cppcheck_setup", run>;

}  // namespace
