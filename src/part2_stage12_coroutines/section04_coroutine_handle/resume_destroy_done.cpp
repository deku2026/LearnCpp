// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section04_coroutine_handle
// Item     : resume_destroy_done
// Topic id : part2/stage12/section04/resume_destroy_done

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section04/resume_destroy_done";

struct Counters {
    int local_destroyed{};
    int promise_destroyed{};
    int body_steps{};
};

struct LocalGuard {
    Counters* counters;
    ~LocalGuard() { ++counters->local_destroyed; }
};

class LifecycleTask {
public:
    struct promise_type {
        Counters* counters;
        std::exception_ptr exception;

        explicit promise_type(Counters& value) noexcept : counters(&value) {}
        ~promise_type() { ++counters->promise_destroyed; }

        LifecycleTask get_return_object() noexcept {
            return LifecycleTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit LifecycleTask(Handle handle) noexcept : handle_(handle) {}
    ~LifecycleTask() {
        if (handle_) {
            handle_.destroy();
        }
    }
    LifecycleTask(const LifecycleTask&) = delete;
    LifecycleTask& operator=(const LifecycleTask&) = delete;
    LifecycleTask(LifecycleTask&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    LifecycleTask& operator=(LifecycleTask&&) = delete;

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

LifecycleTask lifecycle(Counters& counters) {
    LocalGuard local{&counters};
    ++counters.body_steps;
    co_await std::suspend_always{};
    ++counters.body_steps;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    Counters completed;
    {
        auto task = lifecycle(completed);
        LEARN_EXPECT(checks, !task.done());
        task.resume();
        LEARN_EXPECT(checks, !task.done());
        LEARN_EXPECT_EQ(checks, completed.body_steps, 1);
        task.resume();
        LEARN_EXPECT(checks, task.done());
        LEARN_EXPECT_EQ(checks, completed.body_steps, 2);
        LEARN_EXPECT_EQ(checks, completed.local_destroyed, 1);
        // Never resume a done() coroutine; the owner will destroy its final-suspended frame.
    }
    LEARN_EXPECT_EQ(checks, completed.promise_destroyed, 1);

    Counters cancelled;
    {
        auto task = lifecycle(cancelled);
        task.resume();  // Create LocalGuard, then suspend.
        LEARN_EXPECT_EQ(checks, cancelled.local_destroyed, 0);
    }  // Destroying a suspended frame destroys live frame locals exactly once.
    LEARN_EXPECT_EQ(checks, cancelled.local_destroyed, 1);
    LEARN_EXPECT_EQ(checks, cancelled.promise_destroyed, 1);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section04/resume_destroy_done", run>;

}  // namespace
