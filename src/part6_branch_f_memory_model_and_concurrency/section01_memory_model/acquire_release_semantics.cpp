// Runnable teaching example
// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : acquire_release_semantics
// Topic id : part6/f/section01/acquire_release_semantics
// References: C++23 [atomics.order], release sequences

#include "learn/example_support.hpp"

#include <atomic>
#include <string>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part6/f/section01/acquire_release_semantics";

struct Payload {
    int answer{};
    std::string label;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Payload payload;
    std::atomic<int> state{0};
    int observed_answer{};
    std::string observed_label;
    bool relay_advanced{};

    std::thread consumer{[&] {
        int current = state.load(std::memory_order_acquire);
        while (current < 2) {
            state.wait(current, std::memory_order_acquire);
            current = state.load(std::memory_order_acquire);
        }
        // Reading 2 acquires from the release sequence headed by store(1).
        observed_answer = payload.answer;
        observed_label = payload.label;
    }};
    std::thread relay{[&] {
        int current = state.load(std::memory_order_relaxed);
        while (current < 1) {
            state.wait(current, std::memory_order_relaxed);
            current = state.load(std::memory_order_relaxed);
        }
        int expected = 1;
        relay_advanced =
            state.compare_exchange_strong(expected, 2, std::memory_order_relaxed, std::memory_order_relaxed);
        state.notify_all();
    }};
    std::thread producer{[&] {
        payload.answer = 42;
        payload.label = "published before release";
        state.store(1, std::memory_order_release);
        state.notify_all();
    }};

    producer.join();
    relay.join();
    consumer.join();
    LEARN_EXPECT(checks, relay_advanced);
    LEARN_EXPECT_EQ(checks, state.load(std::memory_order_relaxed), 2);
    LEARN_EXPECT_EQ(checks, observed_answer, 42);
    LEARN_EXPECT_EQ(checks, observed_label, std::string{"published before release"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/f/section01/acquire_release_semantics", run>;

}  // namespace
