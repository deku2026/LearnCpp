// LearnCpp topic
// Doc      : 第6部分-支线G · 数据竞争
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : data_race_deep
// Topic id : part6/g/section03/data_race_deep
//
// 要点: 数据竞争 = 冲突的非原子访问且无 happens-before = UB。
// 用 mutex/atomic 消除; 不演示竞争。

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G data race (safe patterns) ===\n";

    // 错误 (不运行): int x=0; 两线程同时 ++x;

    // 修复 1: mutex
    {
        std::mutex m;
        int x = 0;
        std::vector<std::jthread> ts;
        for (int t = 0; t < 4; ++t) {
            ts.emplace_back([&] {
                for (int i = 0; i < 1000; ++i) {
                    std::lock_guard lock(m);
                    ++x;
                }
            });
        }
        ts.clear();
        assert(x == 4000);
    }

    // 修复 2: atomic
    {
        std::atomic<int> x{0};
        std::vector<std::jthread> ts;
        for (int t = 0; t < 4; ++t) {
            ts.emplace_back([&] {
                for (int i = 0; i < 1000; ++i) {
                    x.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
        ts.clear();
        assert(x.load() == 4000);
    }

    std::cout << "  TSan detects data races reliably\n";
    std::cout << "  note: atomic on wrong variable still races other memory\n";
    std::cout << "data_race_deep: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/data_race_deep", run>;

}  // namespace
