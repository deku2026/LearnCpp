// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : clang_tidy_setup
// Topic id : part4/section03/clang_tidy_setup
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section03/clang_tidy_setup";

struct TidyConfig {
    std::set<std::string> enabled;
    std::set<std::string> promoted_to_error;
};

bool valid(const TidyConfig& config) {
    for (const auto& check : config.promoted_to_error)
        if (!config.enabled.contains(check)) return false;
    return config.enabled.contains("bugprone-*") && config.enabled.contains("modernize-*");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const TidyConfig config{{"bugprone-*", "modernize-*", "performance-*"}, {"bugprone-*"}};
    LEARN_EXPECT(checks, valid(config));
    const TidyConfig broken{{"modernize-*"}, {"bugprone-*"}};
    LEARN_EXPECT(checks, !valid(broken));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section03/clang_tidy_setup", run>;

}  // namespace
