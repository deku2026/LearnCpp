// LearnCpp topic
// Doc      : 第6部分-支线F · F1 内存模型基础
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : happens_before_synchronizes_with
// Topic id : part6/f/section01/happens_before_synchronizes_with
//
// 要点: happens-before 保证可见性; synchronizes-with 是跨线程边
//       (典型: release 存储 与 读到该值的 acquire 加载)。
// 参考: [intro.races] [atomics.order] Williams CIA ch.5

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F1 happens-before / synchronizes-with ===\n";

    // 单线程 sequenced-before ⊂ happens-before
    [[maybe_unused]] int local = 0;
    local = 1;
    assert(local == 1);
    std::cout << "  sequenced-before: same-thread program order\n";

    // 经典 publish: 非原子数据 + 原子 flag
    int data = 0;
    std::atomic<bool> ready{false};

    std::jthread producer([&] {
        data = 42;                                     // A
        ready.store(true, std::memory_order_release);  // B release
        // A sequenced-before B
    });

    std::jthread consumer([&] {
        while (!ready.load(std::memory_order_acquire)) {  // C acquire
            std::this_thread::yield();
        }
        // B synchronizes-with C ⇒ A happens-before D
        assert(data == 42);  // D
    });

    // join via jthread dtor
    producer = std::jthread{};
    consumer = std::jthread{};

    std::cout << "  release/acquire on same atomic creates synchronizes-with\n";

    // 多步传递: happens-before 可传递
    std::atomic<int> stage{0};
    int payload = 0;
    std::jthread t1([&] {
        payload = 7;
        stage.store(1, std::memory_order_release);
    });
    std::jthread t2([&] {
        while (stage.load(std::memory_order_acquire) < 1) {
            std::this_thread::yield();
        }
        // 看见 payload 后推进
        assert(payload == 7);
        stage.store(2, std::memory_order_release);
    });
    std::jthread t3([&] {
        while (stage.load(std::memory_order_acquire) < 2) {
            std::this_thread::yield();
        }
        assert(payload == 7);
    });
    t1 = std::jthread{};
    t2 = std::jthread{};
    t3 = std::jthread{};

    std::cout << "  no happens-before + conflicting non-atomic access = data race = UB\n";
    std::cout << "happens_before_synchronizes_with: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/happens_before_synchronizes_with", run>;

}  // namespace
