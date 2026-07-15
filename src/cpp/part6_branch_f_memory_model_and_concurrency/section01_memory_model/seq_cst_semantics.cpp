// LearnCpp topic
// Doc      : 第6部分-支线F · F2.2 seq_cst（验收：讲清五级语义）
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : seq_cst_semantics
// Topic id : part6/f/section01/seq_cst_semantics
//
// 要点: memory_order_seq_cst 是默认；最强也常最贵；提供全局单一全序。
// 无 data race：非原子 data 经 seq_cst flag 同步。
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

    // 发布/订阅（seq_cst 正确但可能过强）
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

    // 专家: 所有 seq_cst 操作参与单一全序 S；
    // 比 acq/rel 多：不同原子变量之间的总序也对所有线程一致
    // 硬件: x86 上 load 常已够强；store 可能多 fence；ARM 更明显
    std::cout << "  seq_cst = acq/rel + single total order of all seq_cst ops\n";
    std::cout << "  guidance: start seq_cst; weaken with proof + TSan\n";
    std::cout << "  cost: extra fences on weakly-ordered ISAs for stores\n";
    std::cout << "seq_cst_semantics: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section01/seq_cst_semantics", run>;

}  // namespace
