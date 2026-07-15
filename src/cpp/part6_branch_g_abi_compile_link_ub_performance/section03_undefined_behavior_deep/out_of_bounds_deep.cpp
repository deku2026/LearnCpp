// LearnCpp topic
// Doc      : 第6部分-支线G · 越界
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : out_of_bounds_deep
// Topic id : part6/g/section03/out_of_bounds_deep
//
// 要点: 数组/容器越界读写下是 UB; 用 at()/size 检查/span。
// 不触发 UB。

#include "learn/topic_registry.hpp"

#include <array>
#include <iostream>
#include <span>
#include <stdexcept>
#include <vector>

namespace {

[[maybe_unused]] int sum_span(std::span<const int> s) {
    int acc = 0;
    for (int v : s) {
        acc += v;
    }
    return acc;
}

[[maybe_unused]] bool index_ok(std::size_t i, std::size_t n) {
    return i < n;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G out-of-bounds (safe patterns) ===\n";

    std::array<int, 3> a{1, 2, 3};
    assert(a[0] == 1);
    assert(a.at(2) == 3);
    try {
        (void)a.at(3);
        assert(false);
    } catch (const std::out_of_range&) {
        std::cout << "  array::at throws on OOB\n";
    }

    std::vector<int> v{10, 20};
    assert(index_ok(1, v.size()));
    assert(!index_ok(2, v.size()));
    assert(sum_span(v) == 30);

    // 危险: v[2] 或 *(a.data()+3) — 不演示
    std::cout << "  ASan catches many heap/stack OOB at runtime\n";
    std::cout << "out_of_bounds_deep: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/out_of_bounds_deep", run>;

}  // namespace
