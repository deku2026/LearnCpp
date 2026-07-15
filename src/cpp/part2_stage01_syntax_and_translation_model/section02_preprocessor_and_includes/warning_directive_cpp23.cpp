// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section02_preprocessor_and_includes
// Item     : warning_directive_cpp23
// Topic id : part2/stage01/section02/warning_directive_cpp23
//
// Covers: C++23 #warning (soft); #error is hard fail (not triggered here)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string_view>

// #warning was a long-standing compiler extension; C++23 standardizes it.
// LEARNCPP_WERROR is OFF by default in this project, so a real #warning is OK.
//
// Soft diagnostic (does not stop the build):
#warning "LearnCpp demo: #warning is a soft diagnostic (C++23 / long-time extension)"

// Hard diagnostic would abort compilation — DO NOT enable for this topic:
// #error "this would fail the build"
//
// Typical hard-fail pattern (commented so the TU still compiles):
// #if __cplusplus < 202302L
// #  error "this project requires C++23"
// #endif

// Model the distinction with ordinary code so demos stay assert-based.
inline constexpr int learncpp_diag_soft = 1;  // #warning class
inline constexpr int learncpp_diag_hard = 2;  // #error class (not executed as directive)

namespace {

void demo_basics() {
    // #warning: compile continues; message appears in compiler output.
    static_assert(learncpp_diag_soft == 1);
    assert(learncpp_diag_soft == 1);

    constexpr std::string_view soft = "warning";
    assert(soft == "warning");
}

void demo_intermediate() {
    // #error: compile stops; use for unrecoverable configuration mistakes.
    // We only document it — triggering #error would break learn_cpp entirely.
    static_assert(learncpp_diag_hard == 2);
    assert(learncpp_diag_hard != learncpp_diag_soft);

    constexpr bool would_error_if_uncommented = true;
    assert(would_error_if_uncommented);
}

void demo_expert() {
    // Choose soft vs hard:
    // - deprecation / temporary portability note → #warning
    // - missing required standard / wrong platform → #error
#if defined(__cplusplus) && __cplusplus >= 202002L
    constexpr bool at_least_cxx20 = true;
#else
    constexpr bool at_least_cxx20 = false;
#endif
    static_assert(at_least_cxx20);
    assert(at_least_cxx20);

    // With -Werror / LEARNCPP_WERROR, #warning becomes a hard failure —
    // this repo keeps that off by default so teaching #warning stays viable.
    assert(learncpp_diag_soft == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/warning_directive_cpp23", run>;

}  // namespace
