// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : warning_treatment_and_werror
// Topic id : part4/section03/warning_treatment_and_werror
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section03/warning_treatment_and_werror";

struct Warning {
    std::string_view owner;
    bool enabled;
};

bool blocks_build(const Warning& warning) {
    return warning.enabled && warning.owner == "project";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT(checks, blocks_build({"project", true}));
    LEARN_EXPECT(checks, !blocks_build({"third-party-system-header", true}));
    LEARN_EXPECT(checks, !blocks_build({"project", false}));
    // Keep project warnings strict while isolating external headers; compiler matrices expose different diagnostics.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section03/warning_treatment_and_werror", run>;

}  // namespace
