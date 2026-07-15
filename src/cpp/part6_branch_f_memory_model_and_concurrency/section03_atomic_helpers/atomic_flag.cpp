// LearnCpp topic
// Doc      : 第6部分-支线F · atomic_flag
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section03_atomic_helpers
// Item     : atomic_flag
// Topic id : part6/f/section03/atomic_flag
//
// 要点: atomic_flag 是唯一保证 lock-free 的原子类型; 自旋锁/一次性标志。
// 参考: cppreference atomic_flag

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

namespace {

class SpinLock {
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;

public:
    void lock() {
        while (flag_.test_and_set(std::memory_order_acquire)) {
            // C++20: flag_.wait(true) 可降低空转; 这里保持可移植自旋
            std::this_thread::yield();
        }
    }
    void unlock() { flag_.clear(std::memory_order_release); }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F3 atomic_flag ===\n";

    std::atomic_flag f = ATOMIC_FLAG_INIT;
    assert(!f.test_and_set());  // 之前 clear, 返回 false
    assert(f.test_and_set());   // 已 set, 返回 true
    f.clear();
    assert(!f.test_and_set());

    SpinLock mtx;
    int counter = 0;
    std::vector<std::jthread> ts;
    for (int t = 0; t < 4; ++t) {
        ts.emplace_back([&] {
            for (int i = 0; i < 1000; ++i) {
                mtx.lock();
                ++counter;
                mtx.unlock();
            }
        });
    }
    ts.clear();
    assert(counter == 4000);

#if defined(__cpp_lib_atomic_flag_test)
    std::atomic_flag g = ATOMIC_FLAG_INIT;
    assert(!g.test(std::memory_order_relaxed));
    g.test_and_set();
    assert(g.test(std::memory_order_relaxed));
    std::cout << "  C++20 test()/wait()/notify available on this lib\n";
#endif

    std::cout << "  guaranteed lock-free; building block for spinlocks\n";
    std::cout << "atomic_flag: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section03/atomic_flag", run>;

}  // namespace
