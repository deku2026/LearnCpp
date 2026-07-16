// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : generator_expressions
// Topic id : part4/section01/generator_expressions
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section01/generator_expressions";

struct BuildContext {
    std::string_view configuration;
    std::string_view compiler;
};

std::string compile_definition(const BuildContext& context) {
    std::string result;
    if (context.configuration == "Debug") result += "LEARN_DEBUG;";
    if (context.compiler == "MSVC") result += "NOMINMAX;";
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, compile_definition({"Debug", "MSVC"}), std::string{"LEARN_DEBUG;NOMINMAX;"});
    LEARN_EXPECT_EQ(checks, compile_definition({"Release", "Clang"}), std::string{});
    // Generator expressions are evaluated for a target/configuration, not as configure-time booleans.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section01/generator_expressions", run>;

}  // namespace
