// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section04_compound_types
// Item     : lvalue_reference
// Topic id : part2/stage02/section04/lvalue_reference
//
// Covers: T& binding, reseating impossible, const T&, ref parameters

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>

namespace {

void add_one(int& n) {
    ++n;
}

int& max_ref(int& a, int& b) {
    return (a > b) ? a : b;
}

void demo_basics() {
    int x = 10;
    int& r = x;
    assert(r == 10);
    r = 20;
    assert(x == 20);

    // r is an alias; address is the same
    assert(&r == &x);
}

void demo_intermediate() {
    int a = 1;
    int b = 2;
    add_one(a);
    assert(a == 2);

    max_ref(a, b) = 100;
    assert(b == 100);

    const int c = 5;
    const int& cr = c;
    assert(cr == 5);

    // const ref can bind temporary (lifetime extended)
    const std::string& s = std::string("temp");
    assert(s == "temp");
}

void demo_expert() {
    int x = 7;
    int& r1 = x;
    // int& r2 = 7; // ill-formed: non-const lvalue ref cannot bind rvalue

    static_assert(std::is_lvalue_reference_v<int&>);
    static_assert(std::is_same_v<decltype(r1), int&>);

    // Reference collapsing preview via typedef
    using RI = int&;
    RI& rr = x;  // still int&
    static_assert(std::is_same_v<decltype(rr), int&>);
    assert(rr == 7);

    // Range-for with references
    int arr[] = {1, 2, 3};
    for (int& e : arr) {
        e *= 2;
    }
    assert(arr[0] == 2 && arr[1] == 4 && arr[2] == 6);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section04/lvalue_reference", run>;

}  // namespace
