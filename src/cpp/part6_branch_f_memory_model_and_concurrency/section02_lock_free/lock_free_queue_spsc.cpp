// LearnCpp topic
// Doc      : 第6部分-支线F · F5 SPSC 队列
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : lock_free_queue_spsc
// Topic id : part6/f/section02/lock_free_queue_spsc
//
// 要点: 单生产者单消费者环形队列无需 CAS; head/tail 用 acquire/release。
// 参考: Williams CIA; LMAX Disruptor 思想 (简化)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

namespace {

template <typename T, std::size_t Cap>
class SpscQueue {
    static_assert((Cap & (Cap - 1)) == 0, "Cap power of two");
    static_assert(Cap >= 2);

    T buf_[Cap]{};
    std::atomic<std::size_t> head_{0};  // 消费者读
    std::atomic<std::size_t> tail_{0};  // 生产者写

public:
    bool try_push(const T& v) {
        const std::size_t t = tail_.load(std::memory_order_relaxed);
        const std::size_t next = (t + 1) & (Cap - 1);
        if (next == head_.load(std::memory_order_acquire)) {
            return false;  // full
        }
        buf_[t] = v;
        tail_.store(next, std::memory_order_release);
        return true;
    }

    bool try_pop(T& out) {
        const std::size_t h = head_.load(std::memory_order_relaxed);
        if (h == tail_.load(std::memory_order_acquire)) {
            return false;  // empty
        }
        out = buf_[h];
        head_.store((h + 1) & (Cap - 1), std::memory_order_release);
        return true;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F5 SPSC lock-free queue ===\n";

    SpscQueue<int, 1024> q;
    constexpr int kN = 10000;
    std::atomic<int> sum{0};

    std::jthread prod([&] {
        for (int i = 1; i <= kN; ++i) {
            while (!q.try_push(i)) {
                std::this_thread::yield();
            }
        }
    });

    std::jthread cons([&] {
        int got = 0;
        int v = 0;
        while (got < kN) {
            if (q.try_pop(v)) {
                sum.fetch_add(v, std::memory_order_relaxed);
                ++got;
            } else {
                std::this_thread::yield();
            }
        }
    });

    prod = std::jthread{};
    cons = std::jthread{};

    const long long expect = static_cast<long long>(kN) * (kN + 1) / 2;
    assert(sum.load() == static_cast<int>(expect) || sum.load() == expect);
    // sum may exceed 32-bit if kN large; kN=10000 OK for int
    assert(static_cast<long long>(sum.load()) == expect);

    std::cout << "  sum=" << sum.load() << " expect=" << expect << '\n';
    std::cout << "  only one producer & one consumer; multi-* needs CAS/MPMC\n";
    std::cout << "lock_free_queue_spsc: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section02/lock_free_queue_spsc", run>;

}  // namespace
