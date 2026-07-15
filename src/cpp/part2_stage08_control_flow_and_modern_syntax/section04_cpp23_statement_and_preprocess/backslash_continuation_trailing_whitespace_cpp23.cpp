// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : backslash_continuation_trailing_whitespace_cpp23
// Topic id : part2/stage08/section04/backslash_continuation_trailing_whitespace_cpp23
//
// Covers: line splicing with backslash; C++23 clarifies trailing whitespace after backslash

#include "learn/topic_registry.hpp"

#include <string>

namespace {

// Clean line continuation (no trailing spaces after \).
#define JOIN_TEXT \
    "hello"       \
    " world"

#define ADD(a, b) ((a) + (b))

void demo_basics() {
    LEARN_CHECK(std::string{JOIN_TEXT} == "hello world");
    LEARN_CHECK((ADD(2, 3) == 5));
}

void demo_intermediate() {
    // Physical lines spliced before tokenization.
    const char* s =
        "ab"
        "cd";
    LEARN_CHECK(std::string{s} == "abcd");
}

void demo_expert() {
    // Teaching: trailing whitespace after \ was a portability hazard;
    // C++23 makes the intent clearer — keep \ as last non-ws character.
    int x = 1 + 2 + 3;
    LEARN_CHECK(x == 6);
    LEARN_CHECK((ADD(10, ADD(1, 1)) == 12));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage08/section04/backslash_continuation_trailing_whitespace_cpp23", run>;

}  // namespace
