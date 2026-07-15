// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section09_aggregates_and_struct_basics
// Item     : aggregate_initialization_rules
// Topic id : part2/stage02/section09/aggregate_initialization_rules
//
// Covers: what is an aggregate; brace elision; no user ctor; public members

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

struct Agg {
    int a;
    int b = 2;  // default member initializer still allows aggregate in C++14+
    int c;
};

struct Nested {
    Agg inner;
    int tag;
};

struct NotAgg {
    int a;
    NotAgg(int v) : a(v) {}  // user constructor => not aggregate
};

void demo_basics() {
    Agg x{1, 2, 3};
    assert(x.a == 1 && x.b == 2 && x.c == 3);

    Agg y{10, 2, 0};  // explicit full init (defaults also apply when omitted carefully)
    assert(y.a == 10);
    assert(y.b == 2);
    assert(y.c == 0);

    static_assert(std::is_aggregate_v<Agg>);
    static_assert(!std::is_aggregate_v<NotAgg>);
}

void demo_intermediate() {
    Nested n{{1, 2, 3}, 9};
    assert(n.inner.a == 1 && n.inner.c == 3);
    assert(n.tag == 9);

    Nested n3{{7, 8, 9}, 1};
    assert(n3.inner.a == 7 && n3.tag == 1);

    int arr[3]{1, 2};  // aggregate array
    assert(arr[0] == 1 && arr[1] == 2 && arr[2] == 0);
}

void demo_expert() {
    // C++20: aggregates can have some public bases in limited forms; keep simple.
    struct Base {
        int b;
    };
    struct Derived : Base {
        int d;
    };
    static_assert(std::is_aggregate_v<Derived>);
    Derived der{{5}, 6};
    assert(der.b == 5 && der.d == 6);

    // NotAgg requires constructor call
    NotAgg na{42};
    assert(na.a == 42);

    // Empty braces value-init all members
    Agg z{};
    assert(z.a == 0 && z.b == 2 && z.c == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section09/aggregate_initialization_rules", run>;

}  // namespace
