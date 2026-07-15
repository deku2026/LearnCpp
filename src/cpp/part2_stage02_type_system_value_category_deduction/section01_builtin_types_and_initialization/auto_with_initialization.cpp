// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : auto_with_initialization
// Topic id : part2/stage02/section01/auto_with_initialization
//
// Covers: auto with =, {}, (), type decay of auto, braced auto special rules

#include "learn/topic_registry.hpp"

#include <initializer_list>
#include <string>
#include <type_traits>
#include <vector>

namespace {

void demo_basics() {
    auto i = 42;
    auto d = 3.14;
    auto b = true;
    auto c = 'x';
    LEARN_CHECK(i == 42);
    LEARN_CHECK(d > 3.0);
    LEARN_CHECK(b);
    LEARN_CHECK(c == 'x');

    static_assert(std::is_same_v<decltype(i), int>);
    static_assert(std::is_same_v<decltype(d), double>);
    static_assert(std::is_same_v<decltype(b), bool>);
    static_assert(std::is_same_v<decltype(c), char>);
}

void demo_intermediate() {
    int x = 10;
    const int cx = 20;
    int& rx = x;

    auto a1 = x;   // int
    auto a2 = cx;  // int (top-level const dropped)
    auto a3 = rx;  // int (reference dropped)
    LEARN_CHECK(a1 == 10 && a2 == 20 && a3 == 10);

    static_assert(std::is_same_v<decltype(a1), int>);
    static_assert(std::is_same_v<decltype(a2), int>);
    static_assert(std::is_same_v<decltype(a3), int>);

    auto& r1 = x;
    const auto& r2 = cx;
    r1 = 11;
    LEARN_CHECK(x == 11);
    LEARN_CHECK(r2 == 20);
    static_assert(std::is_same_v<decltype(r1), int&>);
}

void demo_expert() {
    // auto with braces historically deduced initializer_list; prefer = for scalars.
    auto list = {1, 2, 3};
    static_assert(std::is_same_v<decltype(list), std::initializer_list<int>>);
    LEARN_CHECK(list.size() == 3);

    auto v = std::vector<int>{1, 2, 3};
    LEARN_CHECK(v.size() == 3);

    auto s = std::string{"demo"};
    LEARN_CHECK(s == "demo");

    // auto* and auto&& patterns
    int n = 5;
    auto* p = &n;
    LEARN_CHECK(*p == 5);
    static_assert(std::is_same_v<decltype(p), int*>);

    auto&& uref = n;  // lvalue -> int&
    static_assert(std::is_same_v<decltype(uref), int&>);
    auto&& rref = 42;  // prvalue -> int&&
    static_assert(std::is_same_v<decltype(rref), int&&>);
    LEARN_CHECK(rref == 42);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/auto_with_initialization", run>;

}  // namespace
