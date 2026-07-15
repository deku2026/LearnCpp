// LearnCpp topic
// Doc      : 第6部分-支线F · F2.4 relaxed
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : relaxed_semantics
// Topic id : part6/f/section01/relaxed_semantics
//
// 要点: relaxed 只保证原子性, 不提供同步; 适合独立计数器。
// 参考: cppreference memory_order_relaxed

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F2 relaxed semantics ===\n";

    std::atomic<int> counter{0};
    constexpr int kN = 8;
    constexpr int kPer = 5000;
    std::vector<std::jthread> ts;
    for (int i = 0; i < kN; ++i) {
        ts.emplace_back([&] {
            for (int j = 0; j < kPer; ++j) {
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    ts.clear();
    assert(counter.load(std::memory_order_relaxed) == kN * kPer);
    std::cout << "  concurrent counters: exact total with relaxed RMW\n";

    // 反例教学: relaxed flag 不能同步数据
    // int data=0; atomic<bool> f=false;
    // t1: data=1; f.store(true, relaxed);
    // t2: if(f.load(relaxed)) assert(data==1); // 可能失败 / 数据竞争风险
    std::cout << "  do NOT use relaxed alone to publish non-atomic data\n";

    // 混合: 计数 relaxed, 最终汇总用 acquire/release 或 join 同步
    std::atomic<int> hits{0};
    {
        std::jthread w([&] {
            for (int i = 0; i < 100; ++i) {
                hits.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }  // join
    assert(hits.load(std::memory_order_acquire) == 100);

    std::cout << "relaxed_semantics: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/relaxed_semantics", run>;

}  // namespace
