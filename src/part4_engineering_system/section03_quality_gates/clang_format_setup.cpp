// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : clang_format_setup
// Topic id : part4/section03/clang_format_setup
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section03/clang_format_setup";

std::string normalize_spaces(std::string_view input) {
    std::string output;
    bool previous_space = false;
    for (const char ch : input) {
        const bool space = ch == ' ' || ch == '\t';
        if (!space || !previous_space) output.push_back(space ? ' ' : ch);
        previous_space = space;
    }
    return output;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::string once = normalize_spaces("int   value =  7;");
    const std::string twice = normalize_spaces(once);
    LEARN_EXPECT_EQ(checks, once, std::string{"int value = 7;"});
    LEARN_EXPECT_EQ(checks, twice, once);  // A formatter gate should be deterministic and idempotent.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section03/clang_format_setup", run>;

}  // namespace
