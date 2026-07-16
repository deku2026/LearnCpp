// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : compiler_matrix_gcc_clang_msvc
// Topic id : part4/section06/compiler_matrix_gcc_clang_msvc
// References: official CI, compiler, sanitizer, CMake and standard-library implementation documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <compare>
#include <ranges>
#include <set>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part4/section06/compiler_matrix_gcc_clang_msvc";

struct CompilerLane {
    std::string_view compiler;
    std::string_view standard_library;
    std::string_view warning_profile;
};

constexpr std::array lanes{
    CompilerLane{"gcc", "libstdc++", "-Wall -Wextra -Wpedantic"},
    CompilerLane{"clang", "libc++ or libstdc++", "-Wall -Wextra -Wpedantic"},
    CompilerLane{"msvc", "MSVC STL", "/W4 /permissive-"},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::set<std::string_view> names;
    for (const auto& lane : lanes) names.insert(lane.compiler);
    LEARN_EXPECT_EQ(checks, names.size(), 3U);
    LEARN_EXPECT(checks, lanes[1].standard_library.contains("or"));
    LEARN_EXPECT(checks, lanes[2].warning_profile.contains("/W4"));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section06/compiler_matrix_gcc_clang_msvc", run>;

}  // namespace
