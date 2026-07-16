// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section01_language_basics
// Item     : main_and_program_structure
// Topic id : part2/stage01/section01/main_and_program_structure
// Reference: N4950 [basic.start.main] and [support.start.term].

#include "learn/example_support.hpp"

#include <array>
#include <cstdlib>
#include <string_view>
#include <type_traits>

namespace {

using MainWithoutArguments = int();
using MainWithArguments = int(int, char**);

enum class MainRestriction {
    returns_int,
    cannot_be_overloaded,
    cannot_be_called,
    cannot_be_declared_inline,
    falling_off_end_means_success,
};

constexpr std::array hosted_rules{
    MainRestriction::returns_int,
    MainRestriction::cannot_be_overloaded,
    MainRestriction::cannot_be_called,
    MainRestriction::cannot_be_declared_inline,
    MainRestriction::falling_off_end_means_success,
};

static_assert(std::is_function_v<MainWithoutArguments>);
static_assert(std::is_function_v<MainWithArguments>);
static_assert(std::is_same_v<std::invoke_result_t<MainWithArguments*, int, char**>, int>);

int run(int argc, char** argv) {
    ::learn::ExampleChecks checks{"part2/stage01/section01/main_and_program_structure"};

    char program_name[] = "demo";
    char option[] = "--fast";
    char* simulated_argv[]{program_name, option, nullptr};
    constexpr int simulated_argc = 2;

    LEARN_EXPECT_EQ(checks, std::string_view{simulated_argv[0]}, std::string_view{"demo"});
    LEARN_EXPECT_EQ(checks, std::string_view{simulated_argv[1]}, std::string_view{"--fast"});
    LEARN_EXPECT(checks, simulated_argv[simulated_argc] == nullptr);

    // The standard permits the pointed-to argument strings to be modified.
    option[2] = 'F';
    LEARN_EXPECT_EQ(checks, std::string_view{simulated_argv[1]}, std::string_view{"--Fast"});

    LEARN_EXPECT_EQ(checks, hosted_rules.front(), MainRestriction::returns_int);
    LEARN_EXPECT_EQ(checks, hosted_rules.back(), MainRestriction::falling_off_end_means_success);
    // Do not hard-code the environment's status values; use the standard macros.
    const auto status_for = [](bool success) { return success ? EXIT_SUCCESS : EXIT_FAILURE; };
    LEARN_EXPECT_EQ(checks, status_for(true), EXIT_SUCCESS);
    LEARN_EXPECT_EQ(checks, status_for(false), EXIT_FAILURE);

    const auto out_of_range = ::learn::argument_or(argc, argv, argc, "fallback");
    LEARN_EXPECT_EQ(checks, out_of_range, std::string_view{"fallback"});

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/main_and_program_structure", run>;

}  // namespace
