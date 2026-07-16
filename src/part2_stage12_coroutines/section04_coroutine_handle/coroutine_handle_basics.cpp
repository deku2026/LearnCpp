// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section04_coroutine_handle
// Item     : coroutine_handle_basics
// Topic id : part2/stage12/section04/coroutine_handle_basics

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section04/coroutine_handle_basics";

class CounterSequence {
public:
    struct promise_type {
        int current{};
        std::exception_ptr exception;

        CounterSequence get_return_object() noexcept {
            return CounterSequence{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        std::suspend_always yield_value(int value) noexcept {
            current = value;
            return {};
        }
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit CounterSequence(Handle handle) noexcept : handle_(handle) {}
    ~CounterSequence() {
        if (handle_) {
            handle_.destroy();
        }
    }
    CounterSequence(const CounterSequence&) = delete;
    CounterSequence& operator=(const CounterSequence&) = delete;
    CounterSequence(CounterSequence&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    CounterSequence& operator=(CounterSequence&&) = delete;

    [[nodiscard]] Handle borrow_handle() const noexcept { return handle_; }
    bool next() {
        if (!handle_ || handle_.done()) {
            return false;
        }
        handle_.resume();
        if (handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
        return !handle_.done();
    }
    [[nodiscard]] int value() const noexcept { return handle_.promise().current; }

private:
    Handle handle_{};
};

CounterSequence count() {
    co_yield 10;
    co_yield 20;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    auto sequence = count();
    const CounterSequence::Handle typed = sequence.borrow_handle();
    const std::coroutine_handle<> erased = typed;
    const auto from_address = CounterSequence::Handle::from_address(erased.address());
    const auto from_promise = CounterSequence::Handle::from_promise(typed.promise());
    LEARN_EXPECT(checks, typed);
    LEARN_EXPECT_EQ(checks, typed.address(), erased.address());
    LEARN_EXPECT_EQ(checks, from_address.address(), typed.address());
    LEARN_EXPECT_EQ(checks, from_promise.address(), typed.address());

    LEARN_EXPECT(checks, sequence.next());
    LEARN_EXPECT_EQ(checks, sequence.value(), 10);
    LEARN_EXPECT_EQ(checks, typed.promise().current, 10);
    LEARN_EXPECT(checks, sequence.next());
    LEARN_EXPECT_EQ(checks, sequence.value(), 20);
    LEARN_EXPECT(checks, !sequence.next());
    LEARN_EXPECT(checks, typed.done());
    // All handles above are borrowed; only sequence's destructor destroys the frame.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section04/coroutine_handle_basics", run>;

}  // namespace
