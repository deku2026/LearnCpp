// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section07_import_std_exception
// Item     : import_std_basic_cpp23
// Topic id : part4/section07/import_std_basic_cpp23
// References: official CI, compiler, sanitizer, CMake and standard-library implementation documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <compare>
#include <numeric>
#include <ranges>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section07/import_std_basic_cpp23";

bool standard_module_ready(bool frontend, bool library_artifact, bool build_support) {
    return frontend && library_artifact && build_support;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::vector<int> values{1, 2, 3};
    LEARN_EXPECT_EQ(checks, std::accumulate(values.begin(), values.end(), 0), 6);
    LEARN_EXPECT(checks, standard_module_ready(true, true, true));
    LEARN_EXPECT(checks, !standard_module_ready(true, false, true));
    // The runnable example deliberately uses headers; module syntax is tested only in a separately configured project.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section07/import_std_basic_cpp23", run>;

}  // namespace
