// Runnable teaching example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section01_foundations
// Item     : stackless_state_machine
// Topic id : part2/stage12/section01/stackless_state_machine
// References: C++23 [dcl.fct.def.coroutine], [expr.await], [coro.promise], [coro.handle]

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <string_view>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage12/section01/stackless_state_machine";

class CounterSequence {
public:
    struct promise_type {
        int current{};
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
        void unhandled_exception() const noexcept { std::terminate(); }
    };

    explicit CounterSequence(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    CounterSequence(const CounterSequence&) = delete;
    CounterSequence(CounterSequence&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    ~CounterSequence() {
        if (handle_) {
            handle_.destroy();
        }
    }
    bool next() {
        if (!handle_ || handle_.done()) {
            return false;
        }
        handle_.resume();
        return !handle_.done();
    }
    int value() const { return handle_.promise().current; }

private:
    std::coroutine_handle<promise_type> handle_;
};

CounterSequence count_to(int limit) {
    for (int value = 1; value <= limit; ++value) {
        co_yield value;  // Each suspension point is a state in a compiler-generated stackless machine.
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto sequence = count_to(3);
    std::vector<int> observed;
    while (sequence.next()) {
        observed.push_back(sequence.value());
    }
    LEARN_EXPECT_EQ(checks, observed, std::vector<int>({1, 2, 3}));
    LEARN_EXPECT(checks, !sequence.next());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage12/section01/stackless_state_machine", run>;

}  // namespace
