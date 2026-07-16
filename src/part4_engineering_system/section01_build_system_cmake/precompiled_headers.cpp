// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : precompiled_headers
// Topic id : part4/section01/precompiled_headers
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section01/precompiled_headers";

std::size_t repeated_parse_cost(const std::vector<std::vector<std::string>>& translation_units,
                                const std::set<std::string>& precompiled) {
    std::size_t cost{};
    for (const auto& unit : translation_units) {
        for (const auto& header : unit)
            if (!precompiled.contains(header)) ++cost;
    }
    return cost + precompiled.size();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<std::vector<std::string>> units{{"vector", "string"}, {"vector", "map"}, {"vector", "string"}};
    const auto without = repeated_parse_cost(units, {});
    const auto with = repeated_parse_cost(units, {"vector", "string"});
    LEARN_EXPECT_EQ(checks, without, 6U);
    LEARN_EXPECT_EQ(checks, with, 3U);
    LEARN_EXPECT(checks, with < without);
    // This is a dependency-count model, not a timing claim; actual gains depend on toolchain and source mix.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section01/precompiled_headers", run>;

}  // namespace
