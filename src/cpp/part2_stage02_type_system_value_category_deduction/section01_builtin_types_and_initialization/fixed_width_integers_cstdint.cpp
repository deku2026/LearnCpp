// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : fixed_width_integers_cstdint
// Topic id : part2/stage02/section01/fixed_width_integers_cstdint
//
// Covers: intN_t, uintN_t, least/fast, intptr_t, intmax_t from <cstdint>

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace {

void demo_basics() {
    std::int32_t i32 = -1;
    std::uint32_t u32 = 1u;
    std::int64_t i64 = -2;
    std::uint64_t u64 = 2u;

    assert(i32 == -1);
    assert(u32 == 1u);
    assert(i64 == -2);
    assert(u64 == 2u);
    assert(sizeof(i32) == 4);
    assert(sizeof(u32) == 4);
    assert(sizeof(i64) == 8);
    assert(sizeof(u64) == 8);
}

void demo_intermediate() {
    std::int_least16_t least16 = 1000;
    std::int_fast16_t fast16 = 2000;
    std::uint_least32_t least32 = 3000u;
    std::uint_fast32_t fast32 = 4000u;

    assert(least16 == 1000);
    assert(fast16 == 2000);
    assert(least32 == 3000u);
    assert(fast32 == 4000u);

    static_assert(sizeof(std::int_least16_t) >= 2);
    static_assert(sizeof(std::int_fast16_t) >= 2);
    static_assert(sizeof(std::int_least32_t) >= 4);
    static_assert(sizeof(std::int_fast32_t) >= 4);
}

void demo_expert() {
    std::intptr_t ip = 0;
    std::uintptr_t up = 0;
    std::intmax_t imax = std::numeric_limits<std::intmax_t>::max();
    std::uintmax_t umax = std::numeric_limits<std::uintmax_t>::max();

    int x = 42;
    ip = reinterpret_cast<std::intptr_t>(&x);
    up = reinterpret_cast<std::uintptr_t>(&x);
    assert(reinterpret_cast<int*>(ip) == &x);
    assert(reinterpret_cast<int*>(up) == &x);
    assert(imax > 0);
    assert(umax > 0u);

    static_assert(std::numeric_limits<std::int8_t>::digits == 7);
    static_assert(std::numeric_limits<std::uint8_t>::digits == 8);
    static_assert(std::numeric_limits<std::int16_t>::digits == 15);
    static_assert(std::numeric_limits<std::int32_t>::digits == 31);
    static_assert(std::numeric_limits<std::int64_t>::digits == 63);

    static_assert(std::is_signed_v<std::int32_t>);
    static_assert(std::is_unsigned_v<std::uint32_t>);
    static_assert(sizeof(std::intmax_t) >= sizeof(std::int64_t));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/fixed_width_integers_cstdint", run>;

}  // namespace
