// LearnCpp topic
// Doc      : 第6部分-支线F · F2.2 seq_cst
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : seq_cst_semantics
// Topic id : part6/f/section01/seq_cst_semantics
//
// 要点: memory_order_seq_cst 是默认; 最强也常最贵。
// 参考: cppreference memory_order

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F2 seq_cst semantics ===\n";

    std::atomic<int> n{0};
    n.store(1);  // default seq_cst
    assert(n.load() == 1);
    n.fetch_add(1, std::memory_order_seq_cst);
    assert(n.load(std::memory_order_seq_cst) == 2);

    // 发布也可用 seq_cst (正确但可能过强)
    int data = 0;
    std::atomic<bool> flag{false};
    std::jthread p([&] {
        data = 99;
        flag.store(true, std::memory_order_seq_cst);
    });
    std::jthread c([&] {
        while (!flag.load(std::memory_order_seq_cst)) {
            std::this_thread::yield();
        }
        assert(data == 99);
    });
    p = std::jthread{};
    c = std::jthread{};

    std::cout << "  guidance: start seq_cst; weaken with proof + TSan\n";
    std::cout << "  cost: extra fences on weakly-ordered ISAs for stores\n";
    std::cout << "seq_cst_semantics: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/seq_cst_semantics", run>;

}  // namespace
