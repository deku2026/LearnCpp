// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : unity_build
// Topic id : part4/section01/unity_build
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section01/unity_build";

bool can_merge(const std::vector<std::vector<std::string>>& internal_names) {
    std::set<std::string> seen;
    for (const auto& unit : internal_names) {
        for (const auto& name : unit)
            if (!seen.insert(name).second) return false;
    }
    return true;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT(checks, can_merge({{"parse_a"}, {"parse_b"}}));
    LEARN_EXPECT(checks, !can_merge({{"helper"}, {"helper"}}));
    // Combining sources changes translation-unit boundaries and can expose internal-name/macro collisions.
    LEARN_EXPECT(checks, !can_merge({{"LOCAL_LIMIT"}, {"LOCAL_LIMIT"}}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section01/unity_build", run>;

}  // namespace
