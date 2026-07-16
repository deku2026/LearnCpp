// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : feature_macro_probe_in_ci_cpp23
// Topic id : part4/section06/feature_macro_probe_in_ci_cpp23
// References: official CI, compiler, sanitizer, CMake and standard-library implementation documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <compare>
#include <ranges>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part4/section06/feature_macro_probe_in_ci_cpp23";

struct Probe {
    std::string_view name;
    long long value;
    long long minimum;
};

constexpr std::array probes{
#if defined(__cpp_lib_expected)
    Probe{"expected", __cpp_lib_expected, 202202L},
#else
    Probe{"expected", 0, 202202L},
#endif
#if defined(__cpp_lib_print)
    Probe{"print", __cpp_lib_print, 202207L},
#else
    Probe{"print", 0, 202207L},
#endif
#if defined(__cpp_lib_mdspan)
    Probe{"mdspan", __cpp_lib_mdspan, 202207L},
#else
    Probe{"mdspan", 0, 202207L},
#endif
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, probes.size(), 3U);
    for (const auto& probe : probes) LEARN_EXPECT(checks, probe.value == 0 || probe.value >= probe.minimum);
    // CI should publish the values and gate only facilities a lane promises, rather than assume one C++23 bundle.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section06/feature_macro_probe_in_ci_cpp23", run>;

}  // namespace
