// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section02_preprocessor_and_includes
// Item     : include_guard
// Topic id : part2/stage01/section02/include_guard
//
// Covers: classic #ifndef HEADER_H include guard; same-TU double-include protection

#include "learn/topic_registry.hpp"

#include <cassert>

// Simulate a header body protected by the classic include-guard pattern.
// Real headers wrap their entire content this way so a second #include is a no-op.

#ifndef LEARNCPP_DEMO_MATH_UTILS_HPP_INCLUDED
#define LEARNCPP_DEMO_MATH_UTILS_HPP_INCLUDED

inline constexpr int learncpp_demo_add(int a, int b) {
    return a + b;
}
inline constexpr int learncpp_demo_k = 7;

// First "include" path: mark that we entered the guarded region once.
inline constexpr int learncpp_demo_guard_enter_count = 1;

#endif  // LEARNCPP_DEMO_MATH_UTILS_HPP_INCLUDED

// Second paste of the same header (as if a.cpp included a.hpp and b.hpp both pull it).
// Because the guard macro is already defined, this whole block is skipped.
#ifndef LEARNCPP_DEMO_MATH_UTILS_HPP_INCLUDED
#define LEARNCPP_DEMO_MATH_UTILS_HPP_INCLUDED

// Would redefinition-error if unguarded (same class/inline entity twice in one TU).
inline constexpr int learncpp_demo_add(int a, int b) {
    return a + b;
}
inline constexpr int learncpp_demo_k = 999;
inline constexpr int learncpp_demo_guard_enter_count = 2;

#endif  // LEARNCPP_DEMO_MATH_UTILS_HPP_INCLUDED

// Counter for how many times an unguarded body would re-run (manual model).
#ifndef LEARNCPP_DEMO_UNGUARDED_TICK
#define LEARNCPP_DEMO_UNGUARDED_TICK 1
#else
// Intentionally not redefining — shows why unguarded re-include is dangerous.
#endif

namespace {

void demo_basics() {
    // Guard lets the first inclusion provide declarations/definitions.
    static_assert(learncpp_demo_k == 7);
    assert(learncpp_demo_add(2, 3) == 5);
    assert(learncpp_demo_guard_enter_count == 1);
}

void demo_intermediate() {
    // Second inclusion was skipped: values did not flip to the "unguarded" alternatives.
    static_assert(learncpp_demo_k != 999);
    assert(learncpp_demo_k == 7);
    assert(learncpp_demo_guard_enter_count == 1);

    // After first include, the guard macro is defined for the rest of this TU.
#if defined(LEARNCPP_DEMO_MATH_UTILS_HPP_INCLUDED)
    constexpr bool guard_active = true;
#else
    constexpr bool guard_active = false;
#endif
    static_assert(guard_active);
    assert(guard_active);
}

void demo_expert() {
    // Include guards protect ONE translation unit from redefinition.
    // They do NOT stop the same header from appearing in many TUs (that's ODR/inline).
    constexpr int x = learncpp_demo_add(learncpp_demo_k, 1);
    static_assert(x == 8);
    assert(x == 8);

    // Unique guard names matter: PROJECT_PATH_FILE_HPP style avoids macro collisions.
#if defined(LEARNCPP_DEMO_UNGUARDED_TICK)
    constexpr int unguarded_tick = LEARNCPP_DEMO_UNGUARDED_TICK;
#else
    constexpr int unguarded_tick = 0;
#endif
    assert(unguarded_tick == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/include_guard", run>;

}  // namespace
