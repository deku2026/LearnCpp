// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section07_pitfalls
// Item     : lifetime_management
// Topic id : part2/stage12/section07/lifetime_management

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <stdexcept>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section07/lifetime_management";

struct Counters {
    int body_steps{};
    int local_destroyed{};
    int promise_destroyed{};
};

struct LocalGuard {
    Counters* counters;
    ~LocalGuard() { ++counters->local_destroyed; }
};

class OwnedOperation {
public:
    struct promise_type {
        Counters* counters;
        std::exception_ptr exception;

        promise_type(Counters& value, bool) noexcept : counters(&value) {}
        ~promise_type() { ++counters->promise_destroyed; }

        OwnedOperation get_return_object() noexcept {
            return OwnedOperation{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    OwnedOperation() noexcept = default;
    explicit OwnedOperation(Handle handle) noexcept : handle_(handle) {}
    ~OwnedOperation() { reset(); }
    OwnedOperation(const OwnedOperation&) = delete;
    OwnedOperation& operator=(const OwnedOperation&) = delete;
    OwnedOperation(OwnedOperation&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    OwnedOperation& operator=(OwnedOperation&& other) noexcept {
        if (this != &other) {
            reset();
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }

    [[nodiscard]] bool has_frame() const noexcept { return static_cast<bool>(handle_); }
    [[nodiscard]] bool done() const noexcept { return handle_ && handle_.done(); }

    bool resume() {
        if (!handle_ || handle_.done()) {
            return false;
        }
        handle_.resume();
        if (handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
        return !handle_.done();
    }

    void reset() noexcept {
        if (handle_) {
            handle_.destroy();
            handle_ = {};
        }
    }

private:
    Handle handle_{};
};

OwnedOperation operation(Counters& counters, bool fail) {
    LocalGuard guard{&counters};
    ++counters.body_steps;
    co_await std::suspend_always{};
    ++counters.body_steps;
    if (fail) {
        throw std::runtime_error{"operation failed"};
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    Counters cancelled;
    {
        auto original = operation(cancelled, false);
        auto owner = std::move(original);
        LEARN_EXPECT(checks, !original.has_frame());
        LEARN_EXPECT(checks, owner.has_frame());
        LEARN_EXPECT(checks, owner.resume());
        owner.reset();
        LEARN_EXPECT(checks, !owner.has_frame());
    }
    LEARN_EXPECT_EQ(checks, cancelled.local_destroyed, 1);
    LEARN_EXPECT_EQ(checks, cancelled.promise_destroyed, 1);

    Counters completed;
    {
        auto owner = operation(completed, false);
        LEARN_EXPECT(checks, owner.resume());
        LEARN_EXPECT(checks, !owner.resume());
        LEARN_EXPECT(checks, owner.done());
        LEARN_EXPECT_EQ(checks, completed.local_destroyed, 1);
    }
    LEARN_EXPECT_EQ(checks, completed.promise_destroyed, 1);

    Counters failed;
    {
        auto owner = operation(failed, true);
        LEARN_EXPECT(checks, owner.resume());
        LEARN_EXPECT_THROWS(checks, std::runtime_error, owner.resume());
        LEARN_EXPECT(checks, owner.done());
    }
    LEARN_EXPECT_EQ(checks, failed.local_destroyed, 1);
    LEARN_EXPECT_EQ(checks, failed.promise_destroyed, 1);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section07/lifetime_management", run>;

}  // namespace
