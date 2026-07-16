// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : cross_platform_verification
// Topic id : part4/section06/cross_platform_verification
// References: official CI, compiler, sanitizer, CMake and standard-library implementation documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <compare>
#include <ranges>
#include <set>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part4/section06/cross_platform_verification";

struct Axis {
    std::string_view name;
    std::array<std::string_view, 3> values;
};

constexpr std::array axes{
    Axis{"OS", {"Windows", "Linux", "macOS"}},
    Axis{"architecture", {"x86_64", "arm64", "x86"}},
    Axis{"standard library", {"MSVC STL", "libstdc++", "libc++"}},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, axes.size(), 3U);
    LEARN_EXPECT_EQ(checks, axes[0].values.size(), 3U);
    LEARN_EXPECT(checks, axes[2].values[1] != axes[2].values[2]);
    // Cross-platform confidence requires varying OS, front end, library and architecture—not just one label.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section06/cross_platform_verification", run>;

}  // namespace
