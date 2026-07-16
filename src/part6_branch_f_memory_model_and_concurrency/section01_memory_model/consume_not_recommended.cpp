// Runnable teaching example
// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : consume_not_recommended
// Topic id : part6/f/section01/consume_not_recommended
// References: C++23 [atomics.order], dependency-ordered-before

#include "learn/example_support.hpp"

#include <atomic>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part6/f/section01/consume_not_recommended";

struct Node {
    int value{};
    Node* next{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Node tail{};
    Node head{};
    std::atomic<Node*> published{nullptr};
    int observed_head{};
    int observed_tail{};

    std::thread consumer{[&] {
        Node* current = published.load(std::memory_order_consume);
        while (current == nullptr) {
            published.wait(nullptr, std::memory_order_relaxed);
            current = published.load(std::memory_order_consume);
        }
        // Both reads carry a data dependency from the consumed pointer.
        observed_head = current->value;
        observed_tail = current->next->value;
    }};
    std::thread producer{[&] {
        tail.value = 7;
        head.value = 42;
        head.next = &tail;
        published.store(&head, std::memory_order_release);
        published.notify_one();
    }};
    producer.join();
    consumer.join();

    LEARN_EXPECT_EQ(checks, observed_head, 42);
    LEARN_EXPECT_EQ(checks, observed_tail, 7);

    // Consume is difficult to specify, implement and maintain; implementations
    // commonly give it acquire strength. Prefer acquire unless dependency-only
    // ordering is proven, measured and required.
    Node* acquired = published.load(std::memory_order_acquire);
    LEARN_EXPECT(checks, acquired == &head);
    LEARN_EXPECT_EQ(checks, acquired->next->value, 7);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/f/section01/consume_not_recommended", run>;

}  // namespace
