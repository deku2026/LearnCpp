// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section03_const_constexpr_volatile
// Item     : const_qualifier_pointer_reference
// Topic id : part2/stage02/section03/const_qualifier_pointer_reference
//
// Covers: const objects, pointer-to-const, const pointer, const references

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <string>
#include <type_traits>

namespace {

void demo_basics() {
    const int n = 42;
    assert(n == 42);
    // n = 1; // ill-formed

    int x = 10;
    const int& rx = x;
    assert(rx == 10);
    x = 11;
    assert(rx == 11);  // const ref sees underlying changes
}

void demo_intermediate() {
    int a = 1;
    int b = 2;

    const int* p_to_const = &a;  // cannot modify *p through this pointer
    assert(*p_to_const == 1);
    p_to_const = &b;  // pointer itself can rebind
    assert(*p_to_const == 2);

    int* const const_p = &a;  // pointer cannot rebind
    *const_p = 5;
    assert(a == 5);
    assert(const_p == &a);

    const int* const both = &b;
    assert(*both == 2);
    assert(both == &b);
}

void demo_expert() {
    int v = 7;
    int* p = &v;
    const int* cp = p;  // adding const to pointee is OK
    assert(*cp == 7);

    // Binding temporary to const reference extends lifetime
    const std::size_t& len = std::string("abcd").size();
    assert(len == 4);

    static_assert(std::is_const_v<const int>);
    static_assert(!std::is_const_v<int>);
    static_assert(std::is_same_v<std::remove_const_t<const int>, int>);

    // Function parameter by const ref avoids copies for large objects
    auto sum_two = [](const int& lhs, const int& rhs) { return lhs + rhs; };
    assert(sum_two(3, 4) == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section03/const_qualifier_pointer_reference", run>;

}  // namespace
