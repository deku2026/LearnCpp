// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section07_pitfalls
// Item     : coroutine_frame_allocation
// Topic id : part2/stage12/section07/coroutine_frame_allocation

#include "learn/example_support.hpp"

#include <coroutine>
#include <cstddef>
#include <exception>
#include <new>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section07/coroutine_frame_allocation";

struct AllocationStats {
    int allocations{};
    int deallocations{};
};

struct alignas(std::max_align_t) AllocationHeader {
    AllocationStats* stats;
};

struct FrameGuard {
    int* destroyed;
    ~FrameGuard() { ++*destroyed; }
};

class TrackedFrameTask {
public:
    struct promise_type {
        std::exception_ptr exception;

        static void* operator new(std::size_t size, AllocationStats& stats, int&) {
            auto* header = static_cast<AllocationHeader*>(::operator new(sizeof(AllocationHeader) + size));
            header->stats = &stats;
            ++stats.allocations;
            return header + 1;
        }
        static void operator delete(void* frame, std::size_t) noexcept { release(frame); }
        static void operator delete(void* frame) noexcept { release(frame); }

        TrackedFrameTask get_return_object() noexcept {
            return TrackedFrameTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }

    private:
        static void release(void* frame) noexcept {
            auto* header = static_cast<AllocationHeader*>(frame) - 1;
            ++header->stats->deallocations;
            ::operator delete(header);
        }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit TrackedFrameTask(Handle handle) noexcept : handle_(handle) {}
    ~TrackedFrameTask() {
        if (handle_) {
            handle_.destroy();
        }
    }
    TrackedFrameTask(const TrackedFrameTask&) = delete;
    TrackedFrameTask& operator=(const TrackedFrameTask&) = delete;
    TrackedFrameTask(TrackedFrameTask&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    TrackedFrameTask& operator=(TrackedFrameTask&&) = delete;

    [[nodiscard]] bool done() const noexcept { return handle_.done(); }
    void resume() {
        if (!handle_.done()) {
            handle_.resume();
        }
        if (handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
    }

private:
    Handle handle_{};
};

TrackedFrameTask tracked_frame(AllocationStats& stats, int& local_destroyed) {
    (void)stats;
    FrameGuard guard{&local_destroyed};
    co_await std::suspend_always{};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    AllocationStats stats;
    int local_destroyed{};
    {
        auto task = tracked_frame(stats, local_destroyed);
        LEARN_EXPECT(checks, stats.allocations == 0 || stats.allocations == 1);
        task.resume();
        LEARN_EXPECT(checks, !task.done());
        LEARN_EXPECT_EQ(checks, local_destroyed, 0);
    }
    LEARN_EXPECT_EQ(checks, local_destroyed, 1);
    LEARN_EXPECT_EQ(checks, stats.allocations, stats.deallocations);
    // A zero count is legal if the compiler applies HALO; allocation is not guaranteed observable.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section07/coroutine_frame_allocation", run>;

}  // namespace
