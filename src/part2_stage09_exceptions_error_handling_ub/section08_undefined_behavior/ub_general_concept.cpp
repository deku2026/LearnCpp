// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : ub_general_concept
// Topic id : part2/stage09/section08/ub_general_concept
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <array>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section08/ub_general_concept";

enum class SemanticClass { defined, implementation_defined, unspecified, undefined };

struct Case {
    std::string_view operation;
    SemanticClass classification;
};

constexpr std::array cases{
    Case{"unsigned wraparound", SemanticClass::defined},
    Case{"plain char signedness", SemanticClass::implementation_defined},
    Case{"function argument evaluation order", SemanticClass::unspecified},
    Case{"signed overflow", SemanticClass::undefined},
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, cases.front().classification, SemanticClass::defined);
    LEARN_EXPECT_EQ(checks, cases.back().classification, SemanticClass::undefined);
    // Classification is not a runtime recovery mechanism: undefined operations are never evaluated here.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section08/ub_general_concept", run>;

}  // namespace
