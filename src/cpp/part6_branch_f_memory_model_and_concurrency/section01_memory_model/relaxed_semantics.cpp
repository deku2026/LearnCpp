// LearnCpp topic
// Doc      : 第6部分-支线F · F2 relaxed
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : relaxed_semantics
// Topic id : part6/f/section01/relaxed_semantics
//
// 要点: relaxed 只保证原子性，无同步/顺序；适合计数器等。
// 不把 relaxed 当发布机制。
// 参考: memory_order_relaxed

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F2 relaxed semantics ===\n";

    // 合法: 多线程 atomic 计数（无其他共享非原子）
    std::atomic<int> counter{0};
    std::vector<std::jthread> ts;
    for (int t = 0; t < 4; ++t) {
        ts.emplace_back([&] {
            for (int i = 0; i < 1000; ++i) {
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    ts.clear();
    assert(counter.load(std::memory_order_relaxed) == 4000);

    // 每个原子仍有 modification order（即使 relaxed）
    std::atomic<int> m{0};
    m.store(1, std::memory_order_relaxed);
    m.store(2, std::memory_order_relaxed);
    assert(m.load(std::memory_order_relaxed) == 2);

    // ❌ 不要: data=x; flag.store(true, relaxed);  // 无同步
    std::cout << "  relaxed: atomicity only; no happens-before to other memory\n";
    std::cout << "  OK for independent counters; NOT for publish/subscribe alone\n";
    std::cout << "relaxed_semantics: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/relaxed_semantics", run>;

}  // namespace
