// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : nullptr_replaces_null_zero
// Topic id : part2/stage02/section01/nullptr_replaces_null_zero
//
// Covers: nullptr vs 0/NULL, overload resolution, nullptr_t

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <type_traits>

namespace {

int overload_ptr(int*) {
    return 1;
}
int overload_ptr(std::nullptr_t) {
    return 2;
}
int overload_int(int) {
    return 3;
}

void demo_basics() {
    int* p = nullptr;
    assert(p == nullptr);
    assert(p == 0);  // still comparable, but prefer nullptr in new code

    void* vp = nullptr;
    assert(vp == nullptr);

    static_assert(std::is_same_v<decltype(nullptr), std::nullptr_t>);
}

void demo_intermediate() {
    // 0 is int; nullptr is not an integer type.
    assert(overload_int(0) == 3);
    assert(overload_ptr(nullptr) == 2);

    int x = 1;
    int* px = &x;
    assert(overload_ptr(px) == 1);

    // NULL may be 0 or ((void*)0) depending on platform; nullptr is always null pointer constant of type nullptr_t.
    int* q = NULL;
    assert(q == nullptr);
}

void demo_expert() {
    // Template deduction: nullptr_t is distinct from integral zero.
    auto n = nullptr;
    static_assert(std::is_same_v<decltype(n), std::nullptr_t>);
    static_assert(!std::is_integral_v<std::nullptr_t>);
    static_assert(!std::is_pointer_v<std::nullptr_t>);

    // Conversions: nullptr converts to any pointer type and to bool.
    int* a = nullptr;
    double* b = nullptr;
    bool is_null = static_cast<bool>(a);
    assert(a == nullptr);
    assert(b == nullptr);
    assert(!is_null);

    // Prefer nullptr for pointer null; reserve 0 for integers.
    const int zero = 0;
    assert(zero == 0);
    assert(static_cast<int*>(nullptr) == nullptr);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/nullptr_replaces_null_zero", run>;

}  // namespace
