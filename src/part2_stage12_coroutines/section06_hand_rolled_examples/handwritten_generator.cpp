// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section06_hand_rolled_examples
// Item     : handwritten_generator
// Topic id : part2/stage12/section06/handwritten_generator

#include "learn/example_support.hpp"

#include <coroutine>
#include <cstddef>
#include <exception>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section06/handwritten_generator";

template <class T>
class Generator {
public:
    struct promise_type {
        std::optional<T> current;
        std::exception_ptr exception;

        Generator get_return_object() noexcept {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        std::suspend_always yield_value(T value) noexcept(std::is_nothrow_move_constructible_v<T>) {
            current.emplace(std::move(value));
            return {};
        }
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    Generator() noexcept = default;
    explicit Generator(Handle handle) noexcept : handle_(handle) {}
    ~Generator() { reset(); }
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator(Generator&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            reset();
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }

    class Iterator {
    public:
        using iterator_concept = std::input_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;

        Iterator() noexcept = default;
        explicit Iterator(Handle handle) noexcept : handle_(handle) {}

        const T& operator*() const noexcept { return handle_.promise().current.value(); }
        const T* operator->() const noexcept { return &handle_.promise().current.value(); }
        Iterator& operator++() {
            handle_.resume();
            rethrow_if_exception();
            return *this;
        }
        void operator++(int) { ++*this; }

        friend bool operator==(const Iterator& iterator, std::default_sentinel_t) noexcept {
            return !iterator.handle_ || iterator.handle_.done();
        }

    private:
        void rethrow_if_exception() const {
            if (handle_.promise().exception) {
                std::rethrow_exception(handle_.promise().exception);
            }
        }

        Handle handle_{};
    };

    Iterator begin() {
        if (handle_ && !handle_.done()) {
            handle_.resume();
        }
        if (handle_ && handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
        return Iterator{handle_};
    }
    std::default_sentinel_t end() const noexcept { return {}; }

private:
    void reset() noexcept {
        if (handle_) {
            handle_.destroy();
            handle_ = {};
        }
    }

    Handle handle_{};
};

Generator<int> integers(int first, int last, int* produced) {
    for (int value = first; value < last; ++value) {
        ++*produced;
        co_yield value;
    }
}

struct FrameGuard {
    int* destroyed;
    ~FrameGuard() { ++*destroyed; }
};

Generator<int> guarded(int* destroyed) {
    FrameGuard guard{destroyed};
    co_yield 1;
    co_yield 2;
}

Generator<int> failing() {
    co_yield 1;
    throw std::runtime_error{"generator body failed"};
}

void consume_failure() {
    for ([[maybe_unused]] const int value : failing()) {
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    int produced{};
    auto sequence = integers(3, 7, &produced);
    LEARN_EXPECT_EQ(checks, produced, 0);
    std::vector<int> values;
    for (const int value : sequence) {
        values.push_back(value);
    }
    LEARN_EXPECT_EQ(checks, (values), (std::vector{3, 4, 5, 6}));
    LEARN_EXPECT_EQ(checks, produced, 4);

    int local_destroyed{};
    {
        auto early_stop = guarded(&local_destroyed);
        const auto first = early_stop.begin();
        LEARN_EXPECT_EQ(checks, *first, 1);
    }
    LEARN_EXPECT_EQ(checks, local_destroyed, 1);  // RAII destroys a suspended frame.
    LEARN_EXPECT_THROWS(checks, std::runtime_error, consume_failure());
    static_assert(std::input_iterator<typename Generator<int>::Iterator>);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section06/handwritten_generator", run>;

}  // namespace
