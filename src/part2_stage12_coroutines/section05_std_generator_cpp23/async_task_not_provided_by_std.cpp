// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : async_task_not_provided_by_std
// Topic id : part2/stage12/section05/async_task_not_provided_by_std

#ifdef __has_include
#if __has_include(<generator>)
#include <generator>
#define LEARN_HAS_GENERATOR_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section05/async_task_not_provided_by_std";

class SingleResultTask {
public:
    struct promise_type {
        std::optional<int> result;
        std::exception_ptr exception;
        SingleResultTask get_return_object() noexcept {
            return SingleResultTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_value(int value) noexcept { result = value; }
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit SingleResultTask(Handle handle) noexcept : handle_(handle) {}
    ~SingleResultTask() {
        if (handle_) {
            handle_.destroy();
        }
    }
    SingleResultTask(const SingleResultTask&) = delete;
    SingleResultTask& operator=(const SingleResultTask&) = delete;
    SingleResultTask(SingleResultTask&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    SingleResultTask& operator=(SingleResultTask&&) = delete;

    [[nodiscard]] int run() {
        if (!handle_.done()) {
            handle_.resume();
        }
        if (handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
        return handle_.promise().result.value();
    }

private:
    Handle handle_{};
};

SingleResultTask compute_one(int* executions) {
    ++*executions;
    co_return 42;
}

#if defined(LEARN_HAS_GENERATOR_HEADER) && defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
std::generator<int> produce_many() {
    co_yield 20;
    co_yield 22;
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    int executions{};
    auto task = compute_one(&executions);
    LEARN_EXPECT_EQ(checks, executions, 0);  // This teaching Task is lazy.
    LEARN_EXPECT_EQ(checks, task.run(), 42);
    LEARN_EXPECT_EQ(checks, executions, 1);

#if defined(LEARN_HAS_GENERATOR_HEADER) && defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    std::vector<int> values;
    for (const int value : produce_many()) {
        values.push_back(value);
    }
    LEARN_EXPECT_EQ(checks, (values), (std::vector{20, 22}));
    static_assert(std::ranges::input_range<std::generator<int>>);
    // C++23 standardizes this synchronous sequence type, not an async std::task.
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_generator >= 202207L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/async_task_not_provided_by_std", run>;

}  // namespace
