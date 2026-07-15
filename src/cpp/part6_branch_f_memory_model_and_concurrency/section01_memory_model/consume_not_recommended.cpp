// LearnCpp topic
// Doc      : 第6部分-支线F · F2 consume
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : consume_not_recommended
// Topic id : part6/f/section01/consume_not_recommended
//
// 要点: memory_order_consume 依赖排序；难正确实现，多数编译器提升为 acquire。
// 新代码不要使用 consume。
// 参考: [atomics.order] P0371

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

    // 正确现代写法: acquire
    int* p = nullptr;
    std::atomic<int*> ap{nullptr};
    int storage = 0;

    std::jthread prod([&] {
        storage = 123;
        ap.store(&storage, std::memory_order_release);
    });
    std::jthread cons([&] {
        int* local = nullptr;
        while (!(local = ap.load(std::memory_order_acquire))) {
            std::this_thread::yield();
        }
        assert(*local == 123);
        p = local;
    });
    prod = std::jthread{};
    cons = std::jthread{};
    assert(p && *p == 123);

    // 历史: consume 意图是「只对数据依赖的后续读有序」，比 acquire 轻
    // 现实: 依赖定义太难 → 实现当 acquire；标准倾向弃用路径
    std::cout << "  do NOT use memory_order_consume in new code\n";
    std::cout << "  use acquire (or seq_cst); compilers often promote consume→acquire\n";
    std::cout << "consume_not_recommended: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/consume_not_recommended", run>;

}  // namespace
