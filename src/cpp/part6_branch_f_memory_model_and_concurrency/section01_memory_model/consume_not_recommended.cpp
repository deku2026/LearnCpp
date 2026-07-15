// LearnCpp topic
// Doc      : 第6部分-支线F · F2.1 consume
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : consume_not_recommended
// Topic id : part6/f/section01/consume_not_recommended
//
// 要点: memory_order_consume 本意是数据依赖序; 实现困难,
//       主流实现将其提升为 acquire。新代码不要使用。
// 参考: P0371; cppreference (deprecated for practical use)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F2 consume (not recommended) ===\n";

    // 历史上的 "dependency-ordered-before":
    // release store pointer; consume load pointer; dereference
    // 仅依赖链上的读被同步 —— 极难定义/实现。

    int payload = 0;
    std::atomic<int*> ptr{nullptr};

    std::jthread pub([&] {
        payload = 42;
        ptr.store(&payload, std::memory_order_release);
    });

    std::jthread sub([&] {
        int* p = nullptr;
        while ((p = ptr.load(std::memory_order_acquire)) == nullptr) {
            // 教学: 使用 acquire 代替 consume
            std::this_thread::yield();
        }
        assert(*p == 42);
    });
    pub = std::jthread{};
    sub = std::jthread{};

    // 若写 consume:
    // p = ptr.load(memory_order_consume);
    // 依赖 *p 的读取理论同步, 但独立的其他变量不保证。
    // 实际编译器 ≈ acquire。

    std::cout << "  ISO keeps consume for legacy; implementations strengthen to acquire\n";
    std::cout << "  recommendation: always use acquire for loads that sync\n";
    std::cout << "consume_not_recommended: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/consume_not_recommended", run>;

}  // namespace
