// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section01_language_basics
// Item     : builtin_types_and_literals
// Topic id : part2/stage01/section01/builtin_types_and_literals
//
// Covers: fundamental types, bool/char/nullptr, integer/float suffixes, digit separators, size_t

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace {

void demo_basics() {
    int i = 42;
    unsigned u = 42u;
    long l = 42L;
    long long ll = 42LL;
    bool ok = true;
    char c = 'A';
    double d = 3.14;
    float f = 1.5f;
    void* p = nullptr;

    LEARN_CHECK(i == 42);
    LEARN_CHECK(u == 42u);
    LEARN_CHECK(l == 42L);
    LEARN_CHECK(ll == 42LL);
    LEARN_CHECK(ok);
    LEARN_CHECK(c == 'A');
    LEARN_CHECK(d > 3.0 && d < 3.2);
    LEARN_CHECK(f == 1.5f);
    LEARN_CHECK(p == nullptr);
}

void demo_intermediate() {
    const unsigned long long big = 1'000'000'000'000ull;
    LEARN_CHECK(big == 1000000000000ull);

    const int hex = 0xFF;
    const int oct = 010;
    const int bin = 0b1010;
    LEARN_CHECK(hex == 255);
    LEARN_CHECK(oct == 8);
    LEARN_CHECK(bin == 10);

    const long double ld = 1.0L;
    LEARN_CHECK(ld == 1.0L);

    const std::size_t n = sizeof(int);
    LEARN_CHECK(n >= 2);
    static_assert(std::is_same_v<decltype(sizeof(0)), std::size_t>);

    const char* s = "text";
    LEARN_CHECK(s[0] == 't');
    LEARN_CHECK(nullptr == static_cast<int*>(nullptr));
}

void demo_expert() {
    static_assert(std::numeric_limits<int>::is_integer);
    static_assert(std::numeric_limits<float>::is_iec559 || !std::numeric_limits<float>::is_iec559);

    const std::int32_t i32 = -1;
    const std::uint32_t u32 = 1u;
    LEARN_CHECK(i32 < 0);
    LEARN_CHECK(u32 > 0u);

    // Suffix type checks
    static_assert(std::is_same_v<decltype(1), int>);
    static_assert(std::is_same_v<decltype(1u), unsigned int>);
    static_assert(std::is_same_v<decltype(1L), long>);
    static_assert(std::is_same_v<decltype(1ull), unsigned long long>);
    static_assert(std::is_same_v<decltype(1.0), double>);
    static_assert(std::is_same_v<decltype(1.0f), float>);
    static_assert(std::is_same_v<decltype(1.0L), long double>);
    static_assert(std::is_same_v<decltype(nullptr), std::nullptr_t>);

    // Digit separators are ignored for value
    static_assert(1'000 == 1000);
    static_assert(0xFF'FF == 0xFFFF);

    const std::size_t bytes = sizeof(std::size_t);
    LEARN_CHECK(bytes == 4 || bytes == 8);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/builtin_types_and_literals", run>;

}  // namespace
