// LearnCpp topic
// Doc      : 第2部分-阶段11-并发与多线程.md · 步骤 7.3
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : compare_exchange_cas
// Topic id : part2/stage11/section03/compare_exchange_cas
// Refs     : https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange
//            ISO [atomics.types.operations]
//
// CAS: if current == expected → store desired & return true;
// else write actual into expected & return false. Core of lock-free algos.
// weak may spuriously fail → use in a loop; strong for single-shot.

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

namespace {

void double_with_cas(std::atomic<int>& value) {
    int expected = value.load(std::memory_order_relaxed);
    while (!value.compare_exchange_weak(expected, expected * 2, std::memory_order_relaxed)) {
        // expected updated to current on failure; retry
    }
}

// Minimal Treiber-style lock-free stack (teaching; ignores ABA / reclaim).
struct Node {
    int value;
    Node* next;
};

class LockFreeStack {
    std::atomic<Node*> head_{nullptr};

public:
    ~LockFreeStack() {
        while (Node* n = head_.load(std::memory_order_relaxed)) {
            head_.store(n->next, std::memory_order_relaxed);
            delete n;
        }
    }

    void push(int v) {
        Node* n = new Node{v, nullptr};
        n->next = head_.load(std::memory_order_relaxed);
        while (!head_.compare_exchange_weak(n->next, n, std::memory_order_release, std::memory_order_relaxed)) {
            // n->next reloaded with current head on failure
        }
    }

    bool pop(int& out) {
        Node* n = head_.load(std::memory_order_relaxed);
        while (n && !head_.compare_exchange_weak(n, n->next, std::memory_order_acquire, std::memory_order_relaxed)) {
        }
        if (!n) {
            return false;
        }
        out = n->value;
        delete n;  // teaching only; real systems need safe reclamation
        return true;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [CAS] double_it loop ===\n";
    {
        std::atomic<int> value{3};
        double_with_cas(value);
        assert(value.load() == 6);
        std::cout << "  3 -> 6\n";
    }

    std::cout << "=== strong vs weak ===\n";
    {
        std::atomic<int> x{1};
        int expected = 1;
        const bool ok = x.compare_exchange_strong(expected, 2);
        assert(ok && x.load() == 2);

        expected = 99;  // wrong
        const bool fail = x.compare_exchange_strong(expected, 3);
        assert(!fail);
        assert(expected == 2);  // actual value written back
        assert(x.load() == 2);
        std::cout << "  strong: success path + expected update on fail\n";
    }

    std::cout << "=== concurrent CAS increments ===\n";
    {
        std::atomic<int> x{0};
        auto cas_inc = [&] {
            for (int i = 0; i < 1000; ++i) {
                int e = x.load(std::memory_order_relaxed);
                while (!x.compare_exchange_weak(e, e + 1, std::memory_order_relaxed)) {
                }
            }
        };
        {
            std::jthread t1(cas_inc);
            std::jthread t2(cas_inc);
        }
        assert(x.load() == 2000);
        std::cout << "  x=" << x.load() << '\n';
    }

    std::cout << "=== mini lock-free stack (push/pop CAS) ===\n";
    {
        LockFreeStack st;
        {
            std::vector<std::jthread> pool;
            for (int t = 0; t < 4; ++t) {
                pool.emplace_back([&st, t] {
                    for (int i = 0; i < 100; ++i) {
                        st.push(t * 1000 + i);
                    }
                });
            }
        }
        int count = 0;
        int tmp = 0;
        while (st.pop(tmp)) {
            ++count;
        }
        assert(count == 400);
        std::cout << "  popped " << count << " nodes (ABA omitted — see part6)\n";
    }

    std::cout << "[compare_exchange_cas] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage11/section03/compare_exchange_cas", run>;

}  // namespace
