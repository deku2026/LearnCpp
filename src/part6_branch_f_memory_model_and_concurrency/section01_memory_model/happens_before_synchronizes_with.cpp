// Runnable teaching example
// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section01_memory_model
// Item     : happens_before_synchronizes_with
// Topic id : part6/f/section01/happens_before_synchronizes_with
// References: C++23 [intro.races], [atomics.order], [thread.thread.member]

#include "learn/example_support.hpp"

#include <atomic>
#include <string_view>
#include <thread>

namespace {

constexpr std::string_view kTopic = "part6/f/section01/happens_before_synchronizes_with";

void wait_until_true(const std::atomic<bool>& flag) {
    bool current = flag.load(std::memory_order_acquire);
    while (!current) {
        flag.wait(current, std::memory_order_acquire);
        current = flag.load(std::memory_order_acquire);
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    int source{};
    int transformed{};
    int observed{};
    std::atomic<bool> first_ready{false};
    std::atomic<bool> second_ready{false};

    std::thread final_stage{[&] {
        wait_until_true(second_ready);
        observed = transformed;  // safe through the transitive HB chain
    }};
    std::thread middle_stage{[&] {
        wait_until_true(first_ready);
        transformed = source * 2;
        second_ready.store(true, std::memory_order_release);
        second_ready.notify_one();
    }};
    std::thread first_stage{[&] {
        source = 21;
        first_ready.store(true, std::memory_order_release);
        first_ready.notify_one();
    }};

    first_stage.join();
    middle_stage.join();
    final_stage.join();
    LEARN_EXPECT_EQ(checks, observed, 42);

    // Thread completion synchronizes with a successful join, so this ordinary
    // non-atomic write is safe to read after join returns.
    int written_by_worker{};
    std::thread worker{[&] { written_by_worker = 99; }};
    worker.join();
    LEARN_EXPECT_EQ(checks, written_by_worker, 99);

#if 0
    // Inactive negative form: concurrent non-atomic read/write without HB is a data race and UB.
    int raced = 0;
    std::thread writer([&] { raced = 1; });
    const int unsafe_read = raced;
    writer.join();
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/f/section01/happens_before_synchronizes_with", run>;

}  // namespace
