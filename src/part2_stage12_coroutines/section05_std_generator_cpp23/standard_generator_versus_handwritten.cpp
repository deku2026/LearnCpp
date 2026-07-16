// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : standard_generator_versus_handwritten
// Topic id : part2/stage12/section05/standard_generator_versus_handwritten

#ifdef __has_include
#if __has_include(<generator>)
#include <generator>
#define LEARN_HAS_GENERATOR_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section05/standard_generator_versus_handwritten";

class HandwrittenInts {
public:
    struct promise_type {
        int current{};
        std::exception_ptr exception;
        HandwrittenInts get_return_object() noexcept {
            return HandwrittenInts{std::coroutine_handle<promise_type>::from_promise(*this)};
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
    explicit HandwrittenInts(Handle handle) noexcept : handle_(handle) {}
    ~HandwrittenInts() {
        if (handle_) {
            handle_.destroy();
        }
    }
    HandwrittenInts(const HandwrittenInts&) = delete;
    HandwrittenInts& operator=(const HandwrittenInts&) = delete;
    HandwrittenInts(HandwrittenInts&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    HandwrittenInts& operator=(HandwrittenInts&&) = delete;

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

HandwrittenInts hand_range(int first, int last) {
    for (int value = first; value < last; ++value) {
        co_yield value;
    }
}

[[nodiscard]] std::vector<int> collect_handwritten(HandwrittenInts sequence) {
    std::vector<int> result;
    while (sequence.next()) {
        result.push_back(sequence.value());
    }
    return result;
}

#if defined(LEARN_HAS_GENERATOR_HEADER) && defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
std::generator<int> standard_range(int first, int last) {
    for (int value = first; value < last; ++value) {
        co_yield value;
    }
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};
    const std::vector expected{2, 3, 4, 5};
    LEARN_EXPECT_EQ(checks, collect_handwritten(hand_range(2, 6)), expected);

#if defined(LEARN_HAS_GENERATOR_HEADER) && defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    std::vector<int> standard;
    for (const int value : standard_range(2, 6)) {
        standard.push_back(value);
    }
    LEARN_EXPECT_EQ(checks, standard, expected);
    static_assert(std::ranges::view<std::generator<int>>);
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_generator >= 202207L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/standard_generator_versus_handwritten", run>;

}  // namespace
