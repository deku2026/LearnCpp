// LearnCpp topic
// Doc      : 第6部分-支线F · F6 CAS 循环
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : cas_loop
// Topic id : part6/f/section02/cas_loop
//
// 要点: 无锁更新 = 读旧值 → 算新值 → compare_exchange 失败则重试。
// 参考: [atomics.types.operations]

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

namespace {

// 无锁取 max
void atomic_fetch_max(std::atomic<int>& a, int v) {
    int cur = a.load(std::memory_order_relaxed);
    while (v > cur) {
        if (a.compare_exchange_weak(cur, v, std::memory_order_release, std::memory_order_relaxed)) {
            return;
        }
        // 失败时 cur 已被更新为当前值
    }
}

// 无锁对共享 int 做 "若仍为 expected 则加倍"
[[maybe_unused]] bool try_double(std::atomic<int>& a) {
    int cur = a.load(std::memory_order_relaxed);
    for (;;) {
        if (cur > 1000000) {
            return false;
        }
        int next = cur * 2;
        if (a.compare_exchange_weak(cur, next, std::memory_order_acq_rel, std::memory_order_relaxed)) {
            return true;
        }
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F6 CAS loop ===\n";

    std::atomic<int> m{0};
    std::vector<std::jthread> ts;
    for (int t = 0; t < 4; ++t) {
        ts.emplace_back([&, t] {
            for (int i = 0; i < 100; ++i) {
                atomic_fetch_max(m, t * 100 + i);
            }
        });
    }
    ts.clear();
    assert(m.load() == 3 * 100 + 99);

    [[maybe_unused]] std::atomic<int> x{1};
    assert(try_double(x));
    assert(x.load() == 2);
    assert(try_double(x));
    assert(x.load() == 4);

    std::cout << "  pattern: load; compute; CAS; on failure retry with updated cur\n";
    std::cout << "cas_loop: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section02/cas_loop", run>;

}  // namespace
