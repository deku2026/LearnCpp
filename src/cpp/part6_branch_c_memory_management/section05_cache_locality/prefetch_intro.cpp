// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C9 prefetch)
// Stage    : part6_branch_c_memory_management
// Section  : section05_cache_locality
// Item     : prefetch_intro
// Topic id : part6/c/section05/prefetch_intro
//
// 要点: 软件预取是提示；可移植代码优先靠访问模式/布局，而非内建。
// 本支线铁律：不用编译器扩展；用「提前触摸」模拟预取意图。
// 参考: data locality best practices

#include "learn/topic_registry.hpp"

#include <iostream>
#include <numeric>
#include <vector>

namespace {

// 可移植「预热」：顺序读使硬件预取器工作
[[maybe_unused]] long long touch_sum(const std::vector<int>& v) {
    long long s = 0;
    for (int x : v) s += x;
    return s;
}

// 间接访问（指针追逐）对缓存不友好——对照
long long gather_sum(const std::vector<int>& data, const std::vector<int>& idx) {
    long long s = 0;
    for (int i : idx) s += data[static_cast<std::size_t>(i)];
    return s;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C9 prefetch intro (portable locality) ===\n";

    std::vector<int> data(4096);
    std::iota(data.begin(), data.end(), 0);
    assert(touch_sum(data) == (4095LL * 4096) / 2);

    std::vector<int> idx;
    for (int i = 0; i < 4096; i += 64) idx.push_back(i);
    [[maybe_unused]] long long g = gather_sum(data, idx);
    assert(g > 0);

    std::cout << "  hardware prefetcher loves sequential access\n";
    std::cout << "  software prefetch = non-portable hint; prefer layout/SoA\n";
    std::cout << "  branch C rule: no __builtin_prefetch in examples\n";
    std::cout << "prefetch_intro: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section05/prefetch_intro", run>;

}  // namespace
