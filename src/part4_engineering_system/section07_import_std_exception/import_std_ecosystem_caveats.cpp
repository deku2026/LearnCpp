// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section07_import_std_exception
// Item     : import_std_ecosystem_caveats
// Topic id : part4/section07/import_std_ecosystem_caveats
// References: official CI, compiler, sanitizer, CMake and standard-library implementation documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <compare>
#include <ranges>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part4/section07/import_std_ecosystem_caveats";

struct Risk {
    std::string_view axis;
    std::string_view mitigation;
};

constexpr std::array risks{
    Risk{"compiler/library version coupling", "pin and publish the toolchain"},
    Risk{"prebuilt artifact ABI", "rebuild per toolchain/configuration"},
    Risk{"dependency scanning", "use a generator with supported module graphs"},
    Risk{"IDE/indexer support", "retain a documented header fallback"},
    Risk{"mixed third-party ecosystem", "isolate experiments from the portable target"},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, risks.size(), 5U);
    LEARN_EXPECT(checks, std::ranges::all_of(
                             risks, [](const Risk& risk) { return !risk.axis.empty() && !risk.mitigation.empty(); }));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section07/import_std_ecosystem_caveats", run>;

}  // namespace
