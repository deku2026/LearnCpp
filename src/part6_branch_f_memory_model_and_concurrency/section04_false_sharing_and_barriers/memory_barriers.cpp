// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section04_false_sharing_and_barriers
// Item     : memory_barriers
// Topic id : part6/f/section04/memory_barriers
// Reference: C++23 working draft [atomics.fences], [atomics.wait].

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif

#include <atomic>
#include <thread>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/f/section04/memory_barriers";
    learn::ExampleChecks checks{topic};

#if defined(__cpp_lib_atomic_wait) && __cpp_lib_atomic_wait >= 201907L
    int payload{};
    int observed{};
    int observed_state{};
    std::atomic<int> state{0};

    std::thread consumer{[&] {
        state.wait(0, std::memory_order_relaxed);
        observed_state = state.load(std::memory_order_relaxed);
        // The acquire fence is sequenced after a relaxed load that observed the release sequence.
        std::atomic_thread_fence(std::memory_order_acquire);
        if (observed_state == 1) {
            observed = payload;
        }
    }};
    std::thread producer{[&] {
        payload = 73;
        std::atomic_thread_fence(std::memory_order_release);
        state.store(1, std::memory_order_relaxed);
        state.notify_one();
    }};
    producer.join();
    consumer.join();
    LEARN_EXPECT_EQ(checks, observed_state, 1);
    LEARN_EXPECT_EQ(checks, observed, 73);

    int single_threaded = 5;
    std::atomic_signal_fence(std::memory_order_seq_cst);
    single_threaded += 2;
    LEARN_EXPECT_EQ(checks, single_threaded, 7);

    // thread_fence participates in inter-thread ordering when connected by the required atomic
    // reads/writes; signal_fence only constrains compiler reordering with respect to signal handlers.
    // Neither fence turns an ordinary racing access into an atomic one.
    return checks.result();
#else
    std::atomic<int> state{0};
    state.store(1, std::memory_order_release);
    LEARN_EXPECT_EQ(checks, state.load(std::memory_order_acquire), 1);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_atomic_wait >= 201907L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section04/memory_barriers", run>;

}  // namespace
