// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : cmake_modules_and_import_std_cpp23
// Topic id : part4/section01/cmake_modules_and_import_std_cpp23
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section01/cmake_modules_and_import_std_cpp23";

struct CapabilityAxes {
    bool compiler_frontend;
    bool standard_library_artifact;
    bool build_graph_scanning;
};

bool usable(CapabilityAxes axes) {
    return axes.compiler_frontend && axes.standard_library_artifact && axes.build_graph_scanning;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT(checks, usable({true, true, true}));
    LEARN_EXPECT(checks, !usable({true, false, true}));
    LEARN_EXPECT(checks, !usable({true, true, false}));
    constexpr std::array traditional_headers{"vector", "string", "ranges"};
    LEARN_EXPECT_EQ(checks, traditional_headers.size(), 3U);
    // This repository intentionally builds the header-based path; the module experiment belongs in a separate build.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section01/cmake_modules_and_import_std_cpp23", run>;

}  // namespace
