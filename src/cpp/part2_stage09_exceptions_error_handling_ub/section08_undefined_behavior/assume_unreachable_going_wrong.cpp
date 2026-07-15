// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : assume_unreachable_going_wrong
// Topic id : part2/stage09/section08/assume_unreachable_going_wrong
//
// Covers: false assumptions / reachable unreachable => UB; use asserts instead

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

int clamp_documented(int x, int lo, int hi) {
    // Contract: lo <= hi. Enforce with assert, not assume.
    assert(lo <= hi);
    if (x < lo) {
        return lo;
    }
    if (x > hi) {
        return hi;
    }
    return x;
}

const char* channel_name(int ch) {
    switch (ch) {
        case 0:
            return "L";
        case 1:
            return "R";
        default:
            // SAFE fallback instead of unreachable/assume(false).
            return "?";
    }
}

void demo_basics() {
    assert(clamp_documented(5, 0, 10) == 5);
    assert(clamp_documented(-1, 0, 10) == 0);
    assert(clamp_documented(99, 0, 10) == 10);
}

void demo_intermediate() {
    assert(std::string{channel_name(0)} == "L");
    assert(std::string{channel_name(2)} == "?");
}

void demo_expert() {
    // Teaching: [[assume(x > 0)]] when x can be <= 0 is UB.
    // std::unreachable() on a reachable path is UB.
    // Prefer: validate, return error, or assert in debug.
    int x = 3;
    assert(x > 0);
    assert(clamp_documented(x, 1, 5) == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/assume_unreachable_going_wrong", run>;

}  // namespace
