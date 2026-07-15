// LearnCpp topic example
// Doc      : part6-branch-f-memory-model-concurrency.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : lock_free_stack
// Topic id : part6/f/section02/lock_free_stack
//
// Covers: treiber-style lock-free stack sketch (node reuse avoided)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <memory>

namespace {

struct Node {
    int value;
    Node* next;
};

class LockFreeStack {
    std::atomic<Node*> head{nullptr};

public:
    void push(int v) {
        Node* n = new Node{v, nullptr};
        n->next = head.load(std::memory_order_relaxed);
        while (!head.compare_exchange_weak(n->next, n, std::memory_order_release, std::memory_order_relaxed)) {
        }
    }
    bool pop(int& out) {
        Node* h = head.load(std::memory_order_acquire);
        while (h && !head.compare_exchange_weak(h, h->next, std::memory_order_acq_rel, std::memory_order_acquire)) {
        }
        if (!h) return false;
        out = h->value;
        delete h;  // educational only; real lock-free needs safe reclamation
        return true;
    }
    ~LockFreeStack() {
        int tmp;
        while (pop(tmp)) {
        }
    }
};

void demo_basics() {
    LockFreeStack s;
    s.push(1);
    int v = 0;
    assert(s.pop(v));
    assert(v == 1);
}

void demo_intermediate() {
    LockFreeStack s;
    s.push(1);
    s.push(2);
    int a = 0, b = 0;
    assert(s.pop(a) && s.pop(b));
    assert(a == 2 && b == 1);
}

void demo_expert() {
    LockFreeStack s;
    s.push(5);
    s.push(6);
    int v = 0;
    assert(s.pop(v));
    assert(v == 6);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section02/lock_free_stack", run>;

}  // namespace
