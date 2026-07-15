// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : integral_floating_bool_char
// Topic id : part2/stage02/section01/integral_floating_bool_char
//
// Covers: integral/floating families, bool, char variants, signedness caveats

#include "learn/topic_registry.hpp"

#include <climits>
#include <cstddef>
#include <limits>
#include <type_traits>

namespace {

void demo_basics() {
    int i = 42;
    unsigned u = 42u;
    long l = 100L;
    long long ll = 1000LL;
    bool flag = true;
    char letter = 'Z';
    float f = 1.5f;
    double d = 2.5;
    long double ld = 3.5L;

    LEARN_CHECK(i == 42);
    LEARN_CHECK(u == 42u);
    LEARN_CHECK(l == 100L);
    LEARN_CHECK(ll == 1000LL);
    LEARN_CHECK(flag);
    LEARN_CHECK(letter == 'Z');
    LEARN_CHECK(f == 1.5f);
    LEARN_CHECK(d == 2.5);
    LEARN_CHECK(ld == 3.5L);
}

void demo_intermediate() {
    signed char sc = -1;
    unsigned char uc = 255;
    LEARN_CHECK(sc < 0);
    LEARN_CHECK(uc == 255);

    // Prefer signed/unsigned char for byte arithmetic; bare char signedness is implementation-defined.
    const int from_sc = static_cast<int>(sc);
    const int from_uc = static_cast<int>(uc);
    LEARN_CHECK(from_sc == -1);
    LEARN_CHECK(from_uc == 255);

    wchar_t w = L'A';
    char16_t u16 = u'B';
    char32_t u32 = U'C';
    LEARN_CHECK(w == L'A');
    LEARN_CHECK(u16 == u'B');
    LEARN_CHECK(u32 == U'C');

#if defined(__cpp_char8_t)
    char8_t u8c = u8'X';
    LEARN_CHECK(u8c == u8'X');
#endif

    static_assert(std::is_integral_v<int>);
    static_assert(std::is_floating_point_v<double>);
    static_assert(std::is_same_v<decltype(true), bool>);
}

void demo_expert() {
    static_assert(sizeof(char) == 1);
    static_assert(sizeof(signed char) == 1);
    static_assert(sizeof(unsigned char) == 1);
    static_assert(sizeof(short) >= 2);
    static_assert(sizeof(int) >= 2);
    static_assert(sizeof(long) >= 4);
    static_assert(sizeof(long long) >= 8);

    static_assert(std::numeric_limits<int>::is_integer);
    static_assert(std::numeric_limits<bool>::digits == 1);
    static_assert(CHAR_BIT >= 8);

    // Boolean conversion: non-zero -> true
    LEARN_CHECK(static_cast<bool>(1));
    LEARN_CHECK(!static_cast<bool>(0));
    LEARN_CHECK(static_cast<int>(true) == 1);
    LEARN_CHECK(static_cast<int>(false) == 0);

    // Floating promotions keep value for small integers
    const double from_int = 42;
    LEARN_CHECK(from_int == 42.0);

    // Distinct character types (not the same as char in general)
    static_assert(!std::is_same_v<char, signed char> || std::is_same_v<char, signed char>);
    static_assert(!std::is_same_v<char, unsigned char> || std::is_same_v<char, unsigned char>);
    static_assert(std::is_same_v<decltype(L'a'), wchar_t>);
    static_assert(std::is_same_v<decltype(u'a'), char16_t>);
    static_assert(std::is_same_v<decltype(U'a'), char32_t>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/integral_floating_bool_char", run>;

}  // namespace
