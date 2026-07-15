// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : auto_deduces_decays
// Topic id : part2/stage02/section06/auto_deduces_decays
//
// Covers: auto drops ref/top-level const; array/function decay like template T

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

void demo_basics() {
    int x = 10;
    const int cx = 20;
    int& rx = x;

    auto a = x;
    auto b = cx;
    auto c = rx;
    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), int>);
    static_assert(std::is_same_v<decltype(c), int>);
    assert(a == 10 && b == 20 && c == 10);
}

void demo_intermediate() {
    int arr[3] = {1, 2, 3};
    auto p = arr;  // decays to int*
    static_assert(std::is_same_v<decltype(p), int*>);
    assert(p[0] == 1);

    auto& ref_arr = arr;  // reference to array, no decay
    static_assert(std::is_same_v<decltype(ref_arr), int (&)[3]>);
    assert(ref_arr[2] == 3);

    const int n = 5;
    auto* pn = &n;  // const int*
    static_assert(std::is_same_v<decltype(pn), const int*>);
    assert(*pn == 5);
}

void demo_expert() {
    // Function-to-pointer decay (non-capturing lambda converts to function pointer)
    int (*raw)(int) = [](int v) { return v * 2; };
    auto fpd = raw;
    static_assert(std::is_same_v<decltype(fpd), int (*)(int)>);
    assert(fpd(3) == 6);

    // Preserve const/ref with trailing pattern
    int x = 1;
    const int& cr = x;
    auto copy = cr;            // int
    decltype(auto) keep = cr;  // const int&
    static_assert(std::is_same_v<decltype(copy), int>);
    static_assert(std::is_same_v<decltype(keep), const int&>);
    assert(copy == 1 && keep == 1);

    // auto&& is forwarding reference
    auto&& r1 = x;
    auto&& r2 = 42;
    static_assert(std::is_same_v<decltype(r1), int&>);
    static_assert(std::is_same_v<decltype(r2), int&&>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/auto_deduces_decays", run>;

}  // namespace
