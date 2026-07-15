// LearnCpp topic
// Doc      : 第6部分-支线F · CAS 无锁栈
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : lock_free_stack
// Topic id : part6/f/section02/lock_free_stack
//
// 要点: Treiber stack — 链表头指针 CAS; 教学实现不做内存回收 (hazard/RCU)。
// 参考: R. Treiber 1986; Williams CIA

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <iostream>
#include <optional>
#include <thread>
#include <utility>
#include <vector>

namespace {

template <typename T>
class LockFreeStack {
    struct Node {
        T value;
        Node* next;
        explicit Node(T v) : value(std::move(v)), next(nullptr) {}
    };

    std::atomic<Node*> head_{nullptr};

public:
    ~LockFreeStack() {
        Node* n = head_.load(std::memory_order_relaxed);
        while (n) {
            Node* next = n->next;
            delete n;
            n = next;
        }
    }

    void push(T v) {
        Node* node = new Node(std::move(v));
        node->next = head_.load(std::memory_order_relaxed);
        while (!head_.compare_exchange_weak(node->next, node, std::memory_order_release, std::memory_order_relaxed)) {
        }
    }

    std::optional<T> pop() {
        Node* old = head_.load(std::memory_order_relaxed);
        while (old &&
               !head_.compare_exchange_weak(old, old->next, std::memory_order_acquire, std::memory_order_relaxed)) {
        }
        if (!old) {
            return std::nullopt;
        }
        T v = std::move(old->value);
        // 教学: 直接 delete 在真实并发回收中不安全 (ABA/use-after-free)
        // 这里仅在单测场景于 join 后使用; 并发 pop 下仍可能有风险。
        // 下方测试用限时并发 + 最终排空。
        delete old;
        return v;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F2 lock-free stack (Treiber) ===\n";

    LockFreeStack<int> st;
    st.push(1);
    st.push(2);
    st.push(3);
    assert(st.pop().value() == 3);
    assert(st.pop().value() == 2);
    assert(st.pop().value() == 1);
    assert(!st.pop().has_value());

    // 并发 push, 单线程 pop 排空 (避免教学实现的 reclaim 竞态)
    LockFreeStack<int> st2;
    {
        std::vector<std::jthread> ts;
        for (int t = 0; t < 4; ++t) {
            ts.emplace_back([&, t] {
                for (int i = 0; i < 100; ++i) {
                    st2.push(t * 1000 + i);
                }
            });
        }
    }
    int count = 0;
    while (st2.pop()) {
        ++count;
    }
    assert(count == 400);

    std::cout << "  push/pop via CAS on head; real systems need safe reclamation\n";
    std::cout << "lock_free_stack: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section02/lock_free_stack", run>;

}  // namespace
