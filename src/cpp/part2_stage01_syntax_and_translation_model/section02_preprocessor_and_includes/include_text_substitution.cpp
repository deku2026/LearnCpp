// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section02_preprocessor_and_includes
// Item     : include_text_substitution
// Topic id : part2/stage01/section02/include_text_substitution
//
// Covers: #include is text paste; function-like macro traps; prefer constexpr functions

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string_view>

// Object-like and function-like macros (text substitution only — no scope/types).
#define LEARN_PI 3
#define SQUARE_BAD(x) x* x
#define SQUARE_OK(x) ((x) * (x))

namespace {

// Stand-in for "paste this header text here" (what #include does at phase 4).
constexpr int k_pasted_from_header = 42;

constexpr int square_fn(int x) {
    return x * x;
}

void demo_basics() {
    // #include pastes another file's text into this TU before the compiler parses C++.
    // We model that with a compile-time constant that would live in a header body.
    static_assert(k_pasted_from_header == 42);
    assert(LEARN_PI == 3);

    constexpr int n = SQUARE_OK(5);
    static_assert(n == 25);
    assert(n == 25);
}

void demo_intermediate() {
    // Trap: missing parentheses + operator precedence after text expand.
    // SQUARE_BAD(1 + 2) becomes 1 + 2 * 1 + 2 == 5, not 9.
    constexpr int bad = SQUARE_BAD(1 + 2);
    static_assert(bad == 5);
    assert(bad == 5);

    constexpr int good = SQUARE_OK(1 + 2);
    static_assert(good == 9);
    assert(good == 9);

    // Even with parens, macros still re-expand arguments (side-effect risk).
    // Safe demo: pure values only — never i++ inside a macro argument.
    constexpr int a = 3;
    constexpr int b = 4;
    constexpr int product = SQUARE_OK(a) + SQUARE_OK(b);
    static_assert(product == 25);
    assert(product == 25);
}

void demo_expert() {
    // Modern replacement: constexpr function — typed, scoped, single evaluation.
    static_assert(square_fn(1 + 2) == 9);
    assert(square_fn(1 + 2) == 9);
    assert(square_fn(5) == SQUARE_OK(5));

    // Angle-bracket vs quote includes differ only in search path; both are paste.
    // This TU already used #include "learn/topic_registry.hpp" (quote form).
    constexpr std::string_view note = "include = recursive text paste, not a module import";
    assert(!note.empty());
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/include_text_substitution", run>;

}  // namespace

#undef LEARN_PI
#undef SQUARE_BAD
#undef SQUARE_OK
