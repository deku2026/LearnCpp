// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section02_preprocessor_and_includes
// Item     : warning_directive_cpp23
// Topic id : part2/stage01/section02/warning_directive_cpp23
// References: N4950 [cpp.error]; P2437R1.

#include "learn/example_support.hpp"

#include <array>

// Define this macro in an isolated build to observe the diagnostic while
// allowing preprocessing to continue. The normal test build remains quiet.
#if defined(LEARNCPP_STAGE01_EMIT_DEMONSTRATION_WARNING)
#warning C++23 demonstration: this diagnostic does not require translation to stop
#endif

namespace {

enum class DiagnosticDirective { warning, error };
enum class RequiredOutcome { may_continue, must_fail_preprocessing };

struct DirectiveRule {
    DiagnosticDirective directive;
    RequiredOutcome required_outcome;
    bool standard_in_cpp23;
};

inline constexpr std::array rules{
    DirectiveRule{DiagnosticDirective::warning, RequiredOutcome::may_continue, true},
    DirectiveRule{DiagnosticDirective::error, RequiredOutcome::must_fail_preprocessing, true},
};

// An active #error would intentionally make this translation unit ill-formed:
#if 0
#error demonstration: preprocessing must fail here
#endif

static_assert(rules[0].required_outcome == RequiredOutcome::may_continue);
static_assert(rules[1].required_outcome == RequiredOutcome::must_fail_preprocessing);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section02/warning_directive_cpp23"};
    LEARN_EXPECT_EQ(checks, rules[0].directive, DiagnosticDirective::warning);
    LEARN_EXPECT_EQ(checks, rules[0].required_outcome, RequiredOutcome::may_continue);
    LEARN_EXPECT_EQ(checks, rules[1].directive, DiagnosticDirective::error);
    LEARN_EXPECT_EQ(checks, rules[1].required_outcome, RequiredOutcome::must_fail_preprocessing);
    LEARN_EXPECT(checks, rules[0].standard_in_cpp23 && rules[1].standard_in_cpp23);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/warning_directive_cpp23", run>;

}  // namespace
