// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : decltype_basics
// Topic id : part2/stage02/section06/decltype_basics
//
// Covers: decltype(entity) vs decltype((expr)), value category influence

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>
#include <utility>

namespace {

int global = 3;
int& g() {
    return global;
}

void demo_basics() {
    int x = 10;
    const int cx = 20;

    static_assert(std::is_same_v<decltype(x), int>);
    static_assert(std::is_same_v<decltype(cx), const int>);

    decltype(x) y = 1;
    assert(y == 1);
    decltype(cx) z = 2;
    assert(z == 2);
}

void demo_intermediate() {
    int x = 5;
    int& r = x;

    static_assert(std::is_same_v<decltype(r), int&>);
    static_assert(std::is_same_v<decltype((x)), int&>);   // parenthesized lvalue
    static_assert(std::is_same_v<decltype(x + 1), int>);  // prvalue

    decltype(g()) ref = g();  // int&
    ref = 9;
    assert(global == 9);

    static_assert(std::is_same_v<decltype(1 + 1), int>);
    static_assert(std::is_same_v<decltype(g()), int&>);
}

void demo_expert() {
    int x = 1;
    static_assert(std::is_same_v<decltype(std::move(x)), int&&>);

    int arr[2] = {1, 2};
    static_assert(std::is_same_v<decltype(arr), int[2]>);
    static_assert(std::is_same_v<decltype(arr[0]), int&>);

    // decltype does not evaluate the expression
    int counter = 0;
    using T = decltype(static_cast<int>(counter));
    static_assert(std::is_same_v<T, int>);
    assert(counter == 0);

    const int* p = &x;
    static_assert(std::is_same_v<decltype(p), const int*>);
    static_assert(std::is_same_v<decltype(*p), const int&>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/decltype_basics", run>;

}  // namespace
