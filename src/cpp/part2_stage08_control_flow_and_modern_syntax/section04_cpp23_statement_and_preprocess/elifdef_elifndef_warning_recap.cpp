// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : elifdef_elifndef_warning_recap
// Topic id : part2/stage08/section04/elifdef_elifndef_warning_recap
//
// Covers: #elifdef / #elifndef (C++23) portable recap with classic #elif

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

constexpr int feature_level() {
#if defined(LEARNCPP_FORCE_LEVEL_2)
    return 2;
#elif defined(LEARNCPP_FORCE_LEVEL_1)
    return 1;
#else
    return 0;
#endif
}

// C++23 form (when supported by preprocessor):
// #ifdef A
// #elifdef B
// #elifndef C
// #endif
// We keep classic directives for maximum portability in this demo.

void demo_basics() {
    assert(feature_level() == 0);
}

void demo_intermediate() {
#if defined(__cpp_elifdef) || 1
    // Document intent: elifdef is sugar for #elif defined(X).
    constexpr int via_elif_defined =
#if defined(THIS_MACRO_DOES_NOT_EXIST)
        1
#else
        2
#endif
        ;
    static_assert(via_elif_defined == 2);
    assert(via_elif_defined == 2);
#endif
}

void demo_expert() {
    // #warning is widely available as extension; C++23 standardizes #warning.
    // We do not emit warnings from this demo; only assert control-flow macros.
    assert(feature_level() >= 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section04/elifdef_elifndef_warning_recap", run>;

}  // namespace
