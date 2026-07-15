// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : signed_integer_overflow
// Topic id : part2/stage09/section08/signed_integer_overflow
//
// Covers: signed overflow is UB — detect instead of overflowing

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <limits>
#include <optional>

namespace {

std::optional<int> safe_add(int a, int b) {
    if ((b > 0 && a > std::numeric_limits<int>::max() - b) || (b < 0 && a < std::numeric_limits<int>::min() - b)) {
        return std::nullopt;
    }
    return a + b;
}

std::optional<int> safe_mul(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    // Use wider type for check.
    const std::int64_t r = static_cast<std::int64_t>(a) * static_cast<std::int64_t>(b);
    if (r > std::numeric_limits<int>::max() || r < std::numeric_limits<int>::min()) {
        return std::nullopt;
    }
    return static_cast<int>(r);
}

void demo_basics() {
    assert(safe_add(2, 3).value() == 5);
    assert(!safe_add(std::numeric_limits<int>::max(), 1).has_value());
}

void demo_intermediate() {
    assert(safe_mul(6, 7).value() == 42);
    assert(!safe_mul(std::numeric_limits<int>::max(), 2).has_value());
}

void demo_expert() {
    // Unsigned wrap is defined (mod 2^n); signed overflow is not.
    unsigned u = std::numeric_limits<unsigned>::max();
    assert(u + 1u == 0u);

    // Comment only: `int x = INT_MAX + 1;` is UB — never execute.
    assert(safe_add(std::numeric_limits<int>::min(), -1) == std::nullopt);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/signed_integer_overflow", run>;

}  // namespace
