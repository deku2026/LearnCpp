// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : lock_free_queue_spsc
// Topic id : part6/f/section02/lock_free_queue_spsc
//
// Covers: SPSC ring buffer queue (single producer single consumer)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <thread>
#include <vector>

namespace {

class SpscQueue {
    std::vector<int> buf_;
    std::atomic<std::size_t> head_{0};
    std::atomic<std::size_t> tail_{0};

public:
    explicit SpscQueue(std::size_t cap) : buf_(cap) {}
    bool push(int v) {
        const auto t = tail_.load(std::memory_order_relaxed);
        const auto next = (t + 1) % buf_.size();
        if (next == head_.load(std::memory_order_acquire)) return false;
        buf_[t] = v;
        tail_.store(next, std::memory_order_release);
        return true;
    }
    bool pop(int& out) {
        const auto h = head_.load(std::memory_order_relaxed);
        if (h == tail_.load(std::memory_order_acquire)) return false;
        out = buf_[h];
        head_.store((h + 1) % buf_.size(), std::memory_order_release);
        return true;
    }
};

void demo_basics() {
    SpscQueue q(4);
    LEARN_CHECK(q.push(1));
    int v = 0;
    LEARN_CHECK(q.pop(v) && v == 1);
}

void demo_intermediate() {
    SpscQueue q(8);
    std::thread prod([&] {
        for (int i = 0; i < 5; ++i) {
            while (!q.push(i)) {
            }
        }
    });
    std::thread cons([&] {
        int got = 0;
        int v = 0;
        while (got < 5) {
            if (q.pop(v)) {
                LEARN_CHECK(v == got);
                ++got;
            }
        }
    });
    prod.join();
    cons.join();
}

void demo_expert() {
    SpscQueue q(2);
    LEARN_CHECK(q.push(9));
    LEARN_CHECK(!q.push(8));  // full (one slot reserved)
    int v = 0;
    LEARN_CHECK(q.pop(v) && v == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section02/lock_free_queue_spsc", run>;

}  // namespace
