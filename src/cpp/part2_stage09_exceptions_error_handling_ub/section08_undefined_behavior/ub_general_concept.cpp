// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : ub_general_concept
// Topic id : part2/stage09/section08/ub_general_concept
//
// Covers: what UB means — teach via safe contrasts, never execute UB

#include "learn/topic_registry.hpp"

#include <limits>
#include <optional>

namespace {

// Defined behavior: checked operations.
int safe_div(int a, int b) {
    if (b == 0) {
        return 0;  // policy: defined fallback
    }
    return a / b;
}

std::optional<int> try_div(int a, int b) {
    if (b == 0) {
        return std::nullopt;
    }
    return a / b;
}

void demo_basics() {
    // SAFE: never divide by zero.
    LEARN_CHECK(safe_div(10, 2) == 5);
    LEARN_CHECK(safe_div(10, 0) == 0);
}

void demo_intermediate() {
    auto r = try_div(9, 3);
    LEARN_CHECK(r.has_value() && *r == 3);
    LEARN_CHECK(!try_div(1, 0).has_value());
}

void demo_expert() {
    // Teaching notes (comments only — not executed):
    // - Signed overflow is UB; use checked arithmetic or larger types.
    // - Null deref, use-after-free, data races are UB.
    // - Compilers may assume UB never happens and optimize accordingly.
    int max = std::numeric_limits<int>::max();
    // SAFE contrast: detect would-be overflow instead of overflowing.
    bool would_overflow = max > 0 && max > std::numeric_limits<int>::max() - 1;
    LEARN_CHECK(would_overflow);
    LEARN_CHECK(max == std::numeric_limits<int>::max());
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/ub_general_concept", run>;

}  // namespace
