// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section01_language_basics
// Item     : comments
// Topic id : part2/stage01/section01/comments
//
// Covers: // and /* */ comments; phase 3 comment→whitespace; nesting rules

#include "learn/topic_registry.hpp"

#include <string>

namespace {

void demo_basics() {
    // line comment to end of line
    int a = 1;  // trailing line comment
    /* block comment */
    int b = 2; /* trailing block */
    LEARN_CHECK(a + b == 3);
}

void demo_intermediate() {
    // Block comments do not nest: the first */ ends the comment.
    int x = 10;
    /* outer start
       // line-style text is still inside the block
       x = 999;
    */
    LEARN_CHECK(x == 10);

    // Line comment swallows block openers until newline.
    // /* this is still a line comment
    int y = 20;
    LEARN_CHECK(y == 20);

    /* multi-line
       block spanning
       several lines */
    int z = /* mid-expression */ 5;
    LEARN_CHECK(z == 5);
}

void demo_expert() {
    // Translation phase 3: each comment is replaced by one space.
    // So `a/**/b` becomes `a b` (two tokens), not the identifier `ab`.
    // Token pasting via comments is not a thing — spaces separate tokens.
    int ab = 42;
    int a = 1;
    int b = 2;
    (void)ab;

    // If comments were deleted with no space, a/**/b could become ab.
    // With the required space, a and b stay separate identifiers:
    LEARN_CHECK(a + b == 3);
    LEARN_CHECK(ab == 42);

    // Adjacent string literals still concatenate after comment→space
    // because phase 6 string concatenation ignores intervening whitespace.
    const char* s = "he" /* gap */ "llo";
    LEARN_CHECK(std::string{s} == "hello");

    // Token sequence a /*x*/ b is a, space, b — never the single identifier ab.
    const int left = 3;
    const int right = 4;
    LEARN_CHECK(left + /* ignored */ right == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/comments", run>;

}  // namespace
