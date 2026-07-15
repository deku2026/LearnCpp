// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : assume_attribute_cpp23
// Topic id : part2/stage08/section03/assume_attribute_cpp23
//
// Covers: [[assume(expr)]] (C++23) — teach safely without violating assumptions

#include "learn/topic_registry.hpp"

#include <version>

namespace {

int div_positive(int a, int b) {
    // Precondition documented; we assert instead of relying on assume for safety.
    LEARN_CHECK(b > 0);
#if defined(__cpp_lib_unreachable) || defined(__has_cpp_attribute)
#if __has_cpp_attribute(assume)
    [[assume(b > 0)]];
#endif
#endif
    return a / b;
}

int abs_if_nonneg_documented(int x) {
    LEARN_CHECK(x >= 0);
    return x;
}

void demo_basics() {
    LEARN_CHECK(div_positive(10, 2) == 5);
    LEARN_CHECK(div_positive(9, 3) == 3);
}

void demo_intermediate() {
    LEARN_CHECK(abs_if_nonneg_documented(0) == 0);
    LEARN_CHECK(abs_if_nonneg_documented(7) == 7);
}

void demo_expert() {
    // SAFE teaching: never feed false assumptions. False [[assume]] is UB.
    // Use assert in demos; assume is only a compiler hint when condition holds.
    int a = 8;
    int b = 4;
    LEARN_CHECK(b != 0);
    LEARN_CHECK(div_positive(a, b) == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/assume_attribute_cpp23", run>;

}  // namespace
