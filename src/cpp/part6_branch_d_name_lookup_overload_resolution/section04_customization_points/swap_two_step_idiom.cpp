// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D8 两步 swap)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : swap_two_step_idiom
// Topic id : part6/d/section04/swap_two_step_idiom
//
// 要点: using std::swap; swap(a,b); 先让 ADL 用户版优先，否则 std::swap。
// 演进终点: std::ranges::swap CPO。
// 参考: C++ Coding Standards swap idiom

#include "learn/topic_registry.hpp"

#include <iostream>
#include <utility>

namespace {

namespace lib {
struct Big {
    int id;
    int payload;
};
int custom_swaps = 0;
void swap(Big& a, Big& b) noexcept {
    ++custom_swaps;
    using std::swap;
    swap(a.id, b.id);
    swap(a.payload, b.payload);
}
}  // namespace lib

template <typename T>
void generic_swap(T& a, T& b) {
    using std::swap;  // 两步之①
    swap(a, b);       // 两步之②：ADL + std
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D8 two-step swap idiom ===\n";

    lib::Big a{1, 10}, b{2, 20};
    lib::custom_swaps = 0;
    generic_swap(a, b);
    assert(lib::custom_swaps == 1);
    assert(a.id == 2 && a.payload == 20);
    assert(b.id == 1 && b.payload == 10);

    int x = 3, y = 4;
    generic_swap(x, y);
    assert(x == 4 && y == 3);

    // 直接 std::swap 绕过用户 ADL 版
    lib::custom_swaps = 0;
    std::swap(a, b);
    assert(lib::custom_swaps == 0);

    std::cout << "  step1: using std::swap; step2: unqualified swap(a,b)\n";
    std::cout << "  modern: prefer std::ranges::swap CPO\n";
    std::cout << "swap_two_step_idiom: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section04/swap_two_step_idiom", run>;

}  // namespace
