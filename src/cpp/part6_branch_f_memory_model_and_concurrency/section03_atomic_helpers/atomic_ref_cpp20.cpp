// LearnCpp topic
// Doc      : 第6部分-支线F · F8.1 atomic_ref
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section03_atomic_helpers
// Item     : atomic_ref_cpp20
// Topic id : part6/f/section03/atomic_ref_cpp20
//
// 要点: atomic_ref 对已有非原子对象做原子操作 (须对齐);
//       生命周期内勿混用非原子访问。
// 参考: P0019; cppreference atomic_ref

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

    std::cout << "=== F8 atomic_ref (C++20) ===\n";

    alignas(std::atomic_ref<int>::required_alignment) int plain = 0;

    {
        std::atomic_ref<int> aref(plain);
        assert(aref.is_lock_free() || true);  // may or may not
        aref.store(10);
        assert(aref.load() == 10);
        aref.fetch_add(5);
        assert(aref.load() == 15);
    }
    // atomic_ref 结束后, plain 值为 15
    assert(plain == 15);

    // 多线程通过 atomic_ref 更新同一 plain
    alignas(std::atomic_ref<int>::required_alignment) int shared = 0;
    {
        std::vector<std::jthread> ts;
        for (int t = 0; t < 4; ++t) {
            ts.emplace_back([&] {
                std::atomic_ref<int> r(shared);
                for (int i = 0; i < 1000; ++i) {
                    r.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
    }
    assert(shared == 4000);

    // CAS via ref
    int x = 1;
    std::atomic_ref<int> rx(x);
    [[maybe_unused]] int e = 1;
    assert(rx.compare_exchange_strong(e, 2));
    assert(x == 2);

    std::cout << "  required_alignment=" << std::atomic_ref<int>::required_alignment << '\n';
    std::cout << "  do not race atomic_ref ops with plain non-atomic access\n";
    std::cout << "atomic_ref_cpp20: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section03/atomic_ref_cpp20", run>;

}  // namespace
