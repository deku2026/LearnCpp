// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : multi_config
// Topic id : part4/section01/multi_config
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section01/multi_config";

std::string artifact_path(std::string_view configuration) {
    return "build/" + std::string{configuration} + "/learn_cpp.exe";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr std::array configurations{"Debug", "Release", "RelWithDebInfo"};
    LEARN_EXPECT_EQ(checks, artifact_path(configurations[0]), std::string{"build/Debug/learn_cpp.exe"});
    LEARN_EXPECT_EQ(checks, artifact_path(configurations[1]), std::string{"build/Release/learn_cpp.exe"});
    LEARN_EXPECT(checks, artifact_path(configurations[0]) != artifact_path(configurations[1]));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section01/multi_config", run>;

}  // namespace
