// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section07_pitfalls
// Item     : destructor_cannot_be_a_coroutine
// Topic id : part2/stage12/section07/destructor_cannot_be_a_coroutine

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section07/destructor_cannot_be_a_coroutine";

class CleanupTask {
public:
    struct promise_type {
        std::exception_ptr exception;
        CleanupTask get_return_object() noexcept {
            return CleanupTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit CleanupTask(Handle handle) noexcept : handle_(handle) {}
    ~CleanupTask() {
        if (handle_) {
            handle_.destroy();
        }
    }
    CleanupTask(const CleanupTask&) = delete;
    CleanupTask& operator=(const CleanupTask&) = delete;
    CleanupTask(CleanupTask&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    CleanupTask& operator=(CleanupTask&&) = delete;

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

class Resource {
public:
    explicit Resource(bool& released) noexcept : released_(&released) {}
    ~Resource() noexcept { close_now(); }  // Destruction must remain synchronous and noexcept.

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;

    CleanupTask close_async() {
        co_await std::suspend_always{};  // Represents an externally completed async phase.
        close_now();
    }

private:
    void close_now() noexcept { *released_ = true; }
    bool* released_;
};

#if 0
struct IllFormed {
    ~IllFormed() { co_await std::suspend_always{}; }  // Destructors cannot be coroutines.
};
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    bool released{};
    {
        Resource resource{released};
        auto cleanup = resource.close_async();  // cleanup is destroyed before resource.
        cleanup.resume();
        LEARN_EXPECT(checks, !cleanup.done());
        LEARN_EXPECT(checks, !released);
        cleanup.resume();
        LEARN_EXPECT(checks, cleanup.done());
        LEARN_EXPECT(checks, released);
    }
    LEARN_EXPECT(checks, released);  // The synchronous destructor is idempotent fallback cleanup.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section07/destructor_cannot_be_a_coroutine", run>;

}  // namespace
