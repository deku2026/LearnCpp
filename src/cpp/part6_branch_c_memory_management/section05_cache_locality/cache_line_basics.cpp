// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C9 cache line)
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : cache_line_basics
// Topic id : part6/c/section05/cache_line_basics
//
// 要点: CPU 以缓存行装载；hardware_*_interference_size 标准提示值。
// 参考: [hardware.destructive.interference]

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <iostream>
#include <new>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C9 cache line basics ===\n";

    constexpr std::size_t destr = std::hardware_destructive_interference_size;
    constexpr std::size_t constr = std::hardware_constructive_interference_size;
    std::cout << "  destructive_interference_size=" << destr << '\n';
    std::cout << "  constructive_interference_size=" << constr << '\n';
    assert(destr >= 32);
    assert(constr >= 1);

    // 顺序访问 vs 大步长（教学：验证可运行；非严格计时）
    constexpr int n = 1 << 16;
    std::vector<int> data(n, 1);
    [[maybe_unused]] long long sum = 0;
    for (int i = 0; i < n; ++i) sum += data[i];
    assert(sum == n);

    [[maybe_unused]] long long sum2 = 0;
    for (int i = 0; i < n; i += 16) sum2 += data[i];
    assert(sum2 > 0);

    // 专家: destructive → 避免 false sharing 的填充间距
    // constructive → 希望一起用的数据放同一行
    std::cout << "  sequential access reuses cache lines; large stride less so\n";
    std::cout << "  pad concurrent counters by destructive_interference_size\n";
    std::cout << "cache_line_basics: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/cache_line_basics", run>;

}  // namespace
