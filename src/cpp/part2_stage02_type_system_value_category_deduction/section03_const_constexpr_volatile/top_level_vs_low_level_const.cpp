// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section03_const_constexpr_volatile
// Item     : top_level_vs_low_level_const
// Topic id : part2/stage02/section03/top_level_vs_low_level_const
//
// Covers: top-level const (object itself) vs low-level const (pointee/referee)

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

void demo_basics() {
    // Top-level const: the variable itself is const.
    const int top = 1;
    int value = 2;
    int* const top_ptr = &value;  // top-level const on pointer
    LEARN_CHECK(top == 1);
    *top_ptr = 3;
    LEARN_CHECK(value == 3);
}

void demo_intermediate() {
    int a = 10;
    int b = 20;

    // Low-level const: const applies to the object pointed/referred to.
    const int* low = &a;
    LEARN_CHECK(*low == 10);
    low = &b;  // OK: pointer not top-level const
    LEARN_CHECK(*low == 20);

    // Copying drops top-level const, keeps low-level const.
    const int c = 5;
    int copy = c;  // top-level const of c is ignored in copy
    LEARN_CHECK(copy == 5);

    const int* p = &c;
    const int* q = p;  // low-level const preserved
    LEARN_CHECK(*q == 5);
}

void demo_expert() {
    using TopPtr = int* const;
    using LowPtr = const int*;
    using Both = const int* const;

    static_assert(std::is_const_v<TopPtr>);   // pointer is const
    static_assert(!std::is_const_v<LowPtr>);  // pointer itself not const
    static_assert(std::is_const_v<Both>);

    static_assert(std::is_same_v<std::remove_const_t<TopPtr>, int*>);
    static_assert(std::is_same_v<std::remove_const_t<LowPtr>, const int*>);

    int x = 1;
    TopPtr tp = &x;
    LowPtr lp = &x;
    Both bp = &x;
    *tp = 2;
    LEARN_CHECK(x == 2);
    LEARN_CHECK(*lp == 2);
    LEARN_CHECK(*bp == 2);

    // auto drops top-level const
    const int n = 9;
    auto a = n;
    static_assert(std::is_same_v<decltype(a), int>);
    LEARN_CHECK(a == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section03/top_level_vs_low_level_const", run>;

}  // namespace
