// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section02_preprocessor_and_includes
// Item     : elifdef_cpp23
// Topic id : part2/stage01/section02/elifdef_cpp23
//
// Covers: C++23 #elifdef; portable #elif defined fallback; feature detection

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string_view>

// Scenario macros for the branch ladder (only GPU is "on").
#define LEARNCPP_DEMO_GPU 1
// LEARNCPP_DEMO_CPU intentionally undefined

// Portable path: always valid (pre-C++23 and C++23).
#if defined(LEARNCPP_DEMO_CPU)
inline constexpr int learncpp_backend_portable = 1;  // CPU
#elif defined(LEARNCPP_DEMO_GPU)
inline constexpr int learncpp_backend_portable = 2;  // GPU
#else
inline constexpr int learncpp_backend_portable = 0;
#endif

// C++23 #elifdef is sugar for #elif defined(X). Prefer feature-test when available.
// __cpp_elifdef is not a standard feature-test macro name in all implementations;
// gate on language version (and optional compiler extension support).
#if defined(__cpp_conditional_elifdef) || (defined(__cplusplus) && __cplusplus >= 202302L)
#ifdef LEARNCPP_DEMO_CPU
inline constexpr int learncpp_backend_elifdef = 1;
#elifdef LEARNCPP_DEMO_GPU
inline constexpr int learncpp_backend_elifdef = 2;
#else
inline constexpr int learncpp_backend_elifdef = 0;
#endif
inline constexpr bool learncpp_used_elifdef_syntax = true;
#else
// Fallback identical to #elifdef LEARNCPP_DEMO_GPU
#ifdef LEARNCPP_DEMO_CPU
inline constexpr int learncpp_backend_elifdef = 1;
#elif defined(LEARNCPP_DEMO_GPU)
inline constexpr int learncpp_backend_elifdef = 2;
#else
inline constexpr int learncpp_backend_elifdef = 0;
#endif
inline constexpr bool learncpp_used_elifdef_syntax = false;
#endif

namespace {

void demo_basics() {
    // #ifdef / #elif defined select which block survives phase 4.
    static_assert(learncpp_backend_portable == 2);
    assert(learncpp_backend_portable == 2);
}

void demo_intermediate() {
    // #elifdef X ≡ #elif defined(X) — same branch, shorter spelling in C++23.
    static_assert(learncpp_backend_elifdef == 2);
    assert(learncpp_backend_elifdef == learncpp_backend_portable);

    // Document which spelling this build actually used.
    constexpr std::string_view path = learncpp_used_elifdef_syntax ? "elifdef" : "elif defined";
    assert(path == "elifdef" || path == "elif defined");
}

void demo_expert() {
    // If an old compiler sees unknown #elifdef, it may mis-compile — always
    // keep a #elif defined fallback behind a version/capability gate.
#if defined(LEARNCPP_DEMO_GPU) && !defined(LEARNCPP_DEMO_CPU)
    constexpr bool only_gpu = true;
#else
    constexpr bool only_gpu = false;
#endif
    static_assert(only_gpu);
    assert(only_gpu);
    assert(learncpp_backend_elifdef == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/elifdef_cpp23", run>;

}  // namespace

#undef LEARNCPP_DEMO_GPU
