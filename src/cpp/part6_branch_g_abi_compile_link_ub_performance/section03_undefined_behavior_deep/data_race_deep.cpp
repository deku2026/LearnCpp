// LearnCpp topic
// Doc      : 第6部分-支线G · 数据竞争 / F 内存模型交叉
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : data_race_deep
// Topic id : part6/g/section03/data_race_deep
//
// 要点: 数据竞争 = 冲突的非原子访问且无 happens-before = UB。
// 用 mutex/atomic 消除; 不演示竞争本身。
// 参考: [intro.races] TSan

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

    // 错误模式（不运行）: int x=0; 两线程同时 ++x;

    // 修复 1: mutex → 临界区建立 happens-before
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

    // 修复 3: 不共享可写状态（每线程本地再归约）
    {
        std::atomic<int> total{0};
        std::vector<std::jthread> ts;
        for (int t = 0; t < 4; ++t) {
            ts.emplace_back([&] {
                int local = 0;
                for (int i = 0; i < 1000; ++i) ++local;
                total.fetch_add(local, std::memory_order_relaxed);
            });
        }
        ts.clear();
        assert(total.load() == 4000);
    }

    std::cout << "  definition: conflicting non-atomic access, no happens-before → UB\n";
    std::cout << "  TSan detects data races reliably (correctness, not timing)\n";
    std::cout << "  atomic on wrong variable still races other memory\n";
    std::cout << "data_race_deep: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/data_race_deep", run>;

}  // namespace
