// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : template_argument_deduction
// Topic id : part2/stage02/section06/template_argument_deduction
//
// Covers: function template arg deduction, CTAD intro for simple class templates

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

template <class T>
T id(T v) {
    return v;
}

template <class T>
T& as_lref(T& v) {
    return v;
}

template <class T>
void sink_ptr(T*) {}

template <class T>
struct Pair {
    T a;
    T b;
    Pair(T x, T y) : a(x), b(y) {}
};

void demo_basics() {
    auto a = id(42);
    auto b = id(2.5);
    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), double>);
    assert(a == 42);
    assert(b == 2.5);

    int x = 7;
    int& r = as_lref(x);
    r = 8;
    assert(x == 8);
}

void demo_intermediate() {
    int arr[3] = {1, 2, 3};
    // Array decays when matching T (by value)
    auto first = id(arr[0]);
    assert(first == 1);

    // Pointer parameter: array decays to pointer
    sink_ptr(arr);

    // Explicit template argument overrides deduction
    auto wide = id<long>(3);
    static_assert(std::is_same_v<decltype(wide), long>);
    assert(wide == 3L);

    std::vector<int> v{1, 2, 3};
    auto w = id(v);
    assert(w.size() == 3);
}

void demo_expert() {
    // CTAD: class template argument deduction
    Pair p{1, 2};
    static_assert(std::is_same_v<decltype(p.a), int>);
    assert(p.a == 1 && p.b == 2);

    std::pair pr{std::string{"a"}, 3};
    assert(pr.first == "a");
    assert(pr.second == 3);

    std::array nums{1, 2, 3, 4};
    static_assert(nums.size() == 4);
    assert(nums[3] == 4);

    // Deduction with references: const T& keeps low-level const in T for pointers etc.
    const int c = 9;
    auto ccopy = id(c);  // T=int (top-level const ignored for by-value)
    static_assert(std::is_same_v<decltype(ccopy), int>);
    assert(ccopy == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/template_argument_deduction", run>;

}  // namespace
