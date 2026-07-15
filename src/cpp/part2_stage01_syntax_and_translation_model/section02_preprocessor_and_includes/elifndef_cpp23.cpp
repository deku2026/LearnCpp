// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section02_preprocessor_and_includes
// Item     : elifndef_cpp23
// Topic id : part2/stage01/section02/elifndef_cpp23
//
// Covers: C++23 #elifndef; portable #elif !defined fallback; feature detection

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string_view>

// Prefer low-memory path when LEARNCPP_DEMO_RAM is not defined.
// LEARNCPP_DEMO_RAM intentionally left undefined for this demo.
#define LEARNCPP_DEMO_FAST_PATH 1

// Portable: #elif !defined(X)
#if defined(LEARNCPP_DEMO_RAM)
inline constexpr int learncpp_memory_mode_portable = 1;  // full RAM features
#elif !defined(LEARNCPP_DEMO_RAM)
inline constexpr int learncpp_memory_mode_portable = 2;  // constrained mode
#else
inline constexpr int learncpp_memory_mode_portable = 0;
#endif

// C++23 #elifndef X ≡ #elif !defined(X)
#if defined(__cpp_conditional_elifdef) || (defined(__cplusplus) && __cplusplus >= 202302L)
#ifdef LEARNCPP_DEMO_RAM
inline constexpr int learncpp_memory_mode_elifndef = 1;
#elifndef LEARNCPP_DEMO_RAM
inline constexpr int learncpp_memory_mode_elifndef = 2;
#else
inline constexpr int learncpp_memory_mode_elifndef = 0;
#endif
inline constexpr bool learncpp_used_elifndef_syntax = true;
#else
#ifdef LEARNCPP_DEMO_RAM
inline constexpr int learncpp_memory_mode_elifndef = 1;
#elif !defined(LEARNCPP_DEMO_RAM)
inline constexpr int learncpp_memory_mode_elifndef = 2;
#else
inline constexpr int learncpp_memory_mode_elifndef = 0;
#endif
inline constexpr bool learncpp_used_elifndef_syntax = false;
#endif

// Nested style matching the study doc ladder (#ifdef / #elifdef / #elifndef).
#if defined(__cpp_conditional_elifdef) || (defined(__cplusplus) && __cplusplus >= 202302L)
#ifdef LEARNCPP_DEMO_FAST_PATH
inline constexpr int learncpp_ladder = 10;
#elifdef LEARNCPP_DEMO_RAM
inline constexpr int learncpp_ladder = 20;
#elifndef LEARNCPP_DEMO_SLOW
inline constexpr int learncpp_ladder = 30;
#else
inline constexpr int learncpp_ladder = 0;
#endif
#else
#ifdef LEARNCPP_DEMO_FAST_PATH
inline constexpr int learncpp_ladder = 10;
#elif defined(LEARNCPP_DEMO_RAM)
inline constexpr int learncpp_ladder = 20;
#elif !defined(LEARNCPP_DEMO_SLOW)
inline constexpr int learncpp_ladder = 30;
#else
inline constexpr int learncpp_ladder = 0;
#endif
#endif

namespace {

void demo_basics() {
    // #ifndef / #elif !defined keep a block only when a macro is absent.
    static_assert(learncpp_memory_mode_portable == 2);
    assert(learncpp_memory_mode_portable == 2);
}

void demo_intermediate() {
    static_assert(learncpp_memory_mode_elifndef == 2);
    assert(learncpp_memory_mode_elifndef == learncpp_memory_mode_portable);

    constexpr std::string_view path = learncpp_used_elifndef_syntax ? "elifndef" : "elif !defined";
    assert(path == "elifndef" || path == "elif !defined");
}

void demo_expert() {
    // FAST_PATH is defined → first branch wins; #elifndef is not reached.
    static_assert(learncpp_ladder == 10);
    assert(learncpp_ladder == 10);

#if !defined(LEARNCPP_DEMO_RAM)
    constexpr bool ram_absent = true;
#else
    constexpr bool ram_absent = false;
#endif
    static_assert(ram_absent);
    assert(ram_absent);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/elifndef_cpp23", run>;

}  // namespace

#undef LEARNCPP_DEMO_FAST_PATH
