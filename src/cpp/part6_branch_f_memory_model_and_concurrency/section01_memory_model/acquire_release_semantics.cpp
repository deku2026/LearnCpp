// LearnCpp topic
// Doc      : 第6部分-支线F · F2/F4 acquire-release（验收：何时够用）
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : acquire_release_semantics
// Topic id : part6/f/section01/acquire_release_semantics
//
// 要点: release 发布之前的写；acquire 读到后看到它们。
// 多数「一手写一手读」场景够用且比 seq_cst 便宜。
// 参考: [atomics.order]

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F2/F4 acquire-release semantics ===\n";

    int data = 0;
    int more = 0;
    std::atomic<bool> ready{false};

    std::jthread producer([&] {
        data = 42;  // 非原子
        more = 7;
        ready.store(true, std::memory_order_release);  // 发布 data/more
    });

    std::jthread consumer([&] {
        while (!ready.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
        assert(data == 42);
        assert(more == 7);
    });
    producer = std::jthread{};
    consumer = std::jthread{};

    // RMW 可用 acq_rel
    std::atomic<int> ticket{0};
    int mine = ticket.fetch_add(1, std::memory_order_acq_rel);
    assert(mine == 0);
    assert(ticket.load(std::memory_order_acquire) == 1);

    std::cout << "  release-store pairs with acquire-load on SAME atomic\n";
    std::cout << "  sufficient for one-way publish; cheaper than seq_cst often\n";
    std::cout << "  acq_rel for RMW that both load and store\n";
    std::cout << "acquire_release_semantics: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/acquire_release_semantics", run>;

}  // namespace
