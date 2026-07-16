// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : build_type_matrix_debug_release_relwithdebinfo
// Topic id : part4/section06/build_type_matrix_debug_release_relwithdebinfo
// References: official CI, compiler, sanitizer, CMake and standard-library implementation documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <compare>
#include <ranges>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part4/section06/build_type_matrix_debug_release_relwithdebinfo";

struct BuildType {
    std::string_view name;
    bool optimizes;
    bool emits_debug_info;
    bool defines_ndebug;
};

constexpr std::array types{
    BuildType{"Debug", false, true, false},
    BuildType{"Release", true, false, true},
    BuildType{"RelWithDebInfo", true, true, true},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT(checks, !types[0].optimizes && types[0].emits_debug_info);
    LEARN_EXPECT(checks, types[1].optimizes && types[1].defines_ndebug);
    LEARN_EXPECT(checks, types[2].optimizes && types[2].emits_debug_info);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part4/section06/build_type_matrix_debug_release_relwithdebinfo", run>;

}  // namespace
