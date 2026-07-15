// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : narrowing_check_with_braces
// Topic id : part2/stage02/section01/narrowing_check_with_braces
//
// Covers: brace init rejects narrowing; paren/copy may allow it

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <type_traits>

namespace {

void demo_basics() {
    // Brace initialization forbids narrowing conversions that lose information.
    int exact = {42};
    double wide = {3.0};
    LEARN_CHECK(exact == 42);
    LEARN_CHECK(wide == 3.0);

    // Non-narrowing conversions are fine.
    long from_int{42};
    LEARN_CHECK(from_int == 42L);
}

void demo_intermediate() {
    // Parentheses can allow conversions that braces reject at compile time.
    double d = 3.9;
    int truncated(d);  // allowed: truncates toward zero
    LEARN_CHECK(truncated == 3);

    // Safe narrowing via explicit cast when intentional.
    const int n = static_cast<int>(d);
    LEARN_CHECK(n == 3);

    // Braces accept values that fit exactly.
    std::int16_t s{1000};
    LEARN_CHECK(s == 1000);

    unsigned u{0};
    LEARN_CHECK(u == 0u);
}

void demo_expert() {
    // Constant expressions that fit are not narrowing.
    const int ok_byte{200};  // 200 fits in int
    LEARN_CHECK(ok_byte == 200);

    char c{'A'};
    LEARN_CHECK(c == 'A');

    // Floating to floating of equal or greater range is non-narrowing for constants that fit.
    float f{1.0f};
    double dd{1.0};
    LEARN_CHECK(f == 1.0f);
    LEARN_CHECK(dd == 1.0);

    // Demonstrate intentional conversion path with assert on result.
    const std::uint8_t byte = static_cast<std::uint8_t>(300u % 256u);
    LEARN_CHECK(byte == 44);  // 300 % 256

    static_assert(std::is_same_v<decltype(int{1}), int>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/narrowing_check_with_braces", run>;

}  // namespace
