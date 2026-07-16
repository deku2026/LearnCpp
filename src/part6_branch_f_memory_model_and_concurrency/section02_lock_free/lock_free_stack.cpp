// Runnable teaching example
// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : lock_free_stack
// Topic id : part6/f/section02/lock_free_stack
// References: C++23 [atomics.order], Treiber stack with deferred reclamation

#include "learn/example_support.hpp"

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <memory>
#include <string_view>
#include <thread>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part6/f/section02/lock_free_stack";

struct Node {
    int value{};
    Node* next{};
};

class DeferredTreiberStack {
public:
    // Epoch contract: a node is pushed at most once and is neither reclaimed
    // nor reinserted until all concurrent operations in the epoch have stopped.
    void push(Node* node) noexcept {
        Node* observed = head_.load(std::memory_order_relaxed);
        do {
            node->next = observed;
        } while (!head_.compare_exchange_weak(observed, node, std::memory_order_release, std::memory_order_relaxed));
    }

    Node* pop() noexcept {
        Node* observed = head_.load(std::memory_order_acquire);
        while (observed != nullptr) {
            Node* next = observed->next;
            if (head_.compare_exchange_weak(observed, next, std::memory_order_acquire, std::memory_order_acquire)) {
                return observed;
            }
            // Failure performs an acquire load into observed before the next dereference.
        }
        return nullptr;
    }

    [[nodiscard]] bool empty() const noexcept { return head_.load(std::memory_order_acquire) == nullptr; }

    [[nodiscard]] bool is_lock_free() const noexcept { return head_.is_lock_free(); }

private:
    std::atomic<Node*> head_{nullptr};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr int thread_count = 4;
    constexpr int nodes_per_thread = 200;
    constexpr std::size_t node_count = thread_count * nodes_per_thread;

    // Ownership is retained until every concurrent operation has quiesced.
    // This deferred-reclamation epoch prevents use-after-free and address reuse.
    std::vector<std::unique_ptr<Node>> owners;
    owners.reserve(node_count);
    for (std::size_t index = 0; index < node_count; ++index) {
        owners.push_back(std::make_unique<Node>(static_cast<int>(index + 1), nullptr));
    }

    DeferredTreiberStack stack;
    std::vector<std::thread> pushers;
    pushers.reserve(thread_count);
    for (int thread_index = 0; thread_index < thread_count; ++thread_index) {
        pushers.emplace_back([&, thread_index] {
            const std::size_t first = static_cast<std::size_t>(thread_index * nodes_per_thread);
            const std::size_t last = first + nodes_per_thread;
            for (std::size_t index = first; index < last; ++index) {
                stack.push(owners[index].get());
            }
        });
    }
    for (auto& pusher : pushers) {
        pusher.join();
    }

    std::atomic<std::size_t> popped{0};
    std::vector<int> values(node_count);
    std::vector<std::thread> poppers;
    poppers.reserve(thread_count);
    for (int thread_index = 0; thread_index < thread_count; ++thread_index) {
        poppers.emplace_back([&] {
            while (popped.load(std::memory_order_acquire) < node_count) {
                if (Node* node = stack.pop()) {
                    const std::size_t slot = popped.fetch_add(1, std::memory_order_acq_rel);
                    values[slot] = node->value;
                } else {
                    std::this_thread::yield();
                }
            }
        });
    }
    for (auto& popper : poppers) {
        popper.join();
    }

    std::ranges::sort(values);
    bool all_values_once = true;
    for (std::size_t index = 0; index < values.size(); ++index) {
        all_values_once = all_values_once && values[index] == static_cast<int>(index + 1);
    }
    LEARN_EXPECT_EQ(checks, popped.load(std::memory_order_relaxed), node_count);
    LEARN_EXPECT(checks, stack.empty());
    LEARN_EXPECT(checks, all_values_once);
    if constexpr (std::atomic<Node*>::is_always_lock_free) {
        LEARN_EXPECT(checks, stack.is_lock_free());
    }

#if 0
    // Inactive dangerous form: deleting immediately after pop lets another
    // contender dereference freed memory. Use hazards/epochs/RCU or defer deletion.
    if (Node* node = stack.pop()) {
        delete node;
    }
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/f/section02/lock_free_stack", run>;

}  // namespace
