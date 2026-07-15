// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.5.4 perf / VTune / Tracy
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : profilers_overview
// Topic id : part4/section05/profilers_overview
//
// 要点: 采样剖析找热点; 先测再优化。演示计时对比两种实现。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace {

using clock = std::chrono::steady_clock;

template <class F>
std::int64_t time_us(F&& f, int rounds) {
    auto t0 = clock::now();
    for (int i = 0; i < rounds; ++i) {
        f();
    }
    return std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - t0).count();
}

// 慢: 二次求和
std::uint64_t sum_slow(const std::vector<int>& v) {
    std::uint64_t s = 0;
    for (std::size_t i = 0; i < v.size(); ++i) {
        for (std::size_t j = 0; j <= i; ++j) {
            s += static_cast<std::uint64_t>(v[j]);
        }
    }
    return s;
}

// 快: 前缀一次
std::uint64_t sum_fast(const std::vector<int>& v) {
    std::uint64_t s = 0;
    std::uint64_t prefix = 0;
    for (int x : v) {
        prefix += static_cast<std::uint64_t>(x);
        s += prefix;
    }
    return s;
}

struct Hotspot {
    std::string name;
    std::int64_t samples;
};

// 归一化「采样占比」
double pct(const Hotspot& h, std::int64_t total) {
    return total == 0 ? 0.0 : 100.0 * static_cast<double>(h.samples) / static_cast<double>(total);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== profilers_overview ===\n";

    std::vector<int> data(200, 1);
    [[maybe_unused]] auto slow = sum_slow(data);
    [[maybe_unused]] auto fast = sum_fast(data);
    assert(slow == fast);

    const int rounds = 50;
    auto t_slow = time_us([&] { (void)sum_slow(data); }, rounds);
    auto t_fast = time_us([&] { (void)sum_fast(data); }, rounds);
    std::cout << "  timed slow=" << t_slow << "us fast=" << t_fast << "us\n";
    // 不强制 slow>fast(优化器可能很强), 但结果必须一致
    assert(slow == fast);

    // 采样剖析心智: 热点列表
    std::vector<Hotspot> stack{
        {"sum_slow", 800},
        {"malloc", 50},
        {"other", 150},
    };
    std::int64_t total = 0;
    for (const auto& h : stack) {
        total += h.samples;
    }
    assert(pct(stack[0], total) > 50.0);
    std::cout << "  hotspot " << stack[0].name << " ~" << pct(stack[0], total) << "%\n";

    // 工具: perf record/report; VTune; Tracy 帧
    std::cout << "  tools: perf (Linux), VTune, Tracy (realtime)\n";
    std::cout << "profilers_overview: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/profilers_overview", run>;

}  // namespace
