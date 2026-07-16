// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : pvs_studio_intro
// Topic id : part4/section03/pvs_studio_intro
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section03/pvs_studio_intro";

enum class Severity { note, warning, error };
struct Diagnostic {
    std::string_view code;
    Severity severity;
    bool new_in_diff;
};

constexpr std::array diagnostics{
    Diagnostic{"V501", Severity::error, true},
    Diagnostic{"V1048", Severity::warning, false},
    Diagnostic{"V2008", Severity::note, true},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto blocking = std::ranges::count_if(diagnostics, [](const Diagnostic& diagnostic) {
        return diagnostic.new_in_diff && diagnostic.severity == Severity::error;
    });
    LEARN_EXPECT_EQ(checks, blocking, 1);
    LEARN_EXPECT_EQ(checks, diagnostics.size(), 3U);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section03/pvs_studio_intro", run>;

}  // namespace
