// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section06_hand_rolled_examples
// Item     : handwritten_generator
// Topic id : part2/stage12/section06/handwritten_generator
//
// 要点: 完整手写 Generator<T> — promise + handle RAII + 迭代器驱动 resume。
// Refs:
//   https://en.cppreference.com/w/cpp/language/coroutines
//   文档步骤 7

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {

template <typename T>
class Generator {
public:
    struct promise_type {
        T value_{};
        std::exception_ptr exception_{};

        Generator get_return_object() { return Generator{Handle::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            value_ = std::move(value);
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { exception_ = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit Generator(Handle h) : handle_(h) {}
    ~Generator() {
        if (handle_) {
            handle_.destroy();
        }
    }
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator(Generator&& o) noexcept : handle_(std::exchange(o.handle_, {})) {}
    Generator& operator=(Generator&& o) noexcept {
        if (this != &o) {
            if (handle_) {
                handle_.destroy();
            }
            handle_ = std::exchange(o.handle_, {});
        }
        return *this;
    }

    class Iterator {
        Handle handle_{};

    public:
        explicit Iterator(Handle h) : handle_(h) {}
        void operator++() {
            handle_.resume();
            if (handle_.promise().exception_) {
                std::rethrow_exception(handle_.promise().exception_);
            }
        }
        const T& operator*() const { return handle_.promise().value_; }
        bool operator==(std::default_sentinel_t) const { return !handle_ || handle_.done(); }
    };

    Iterator begin() {
        if (handle_) {
            handle_.resume();  // 启动到第一个 co_yield
            if (handle_.promise().exception_) {
                std::rethrow_exception(handle_.promise().exception_);
            }
        }
        return Iterator{handle_};
    }
    std::default_sentinel_t end() { return {}; }

private:
    Handle handle_{};
};

Generator<int> range(int from, int to) {
    for (int i = from; i < to; ++i) {
        co_yield i;
    }
}

Generator<int> failing() {
    co_yield 1;
    throw std::runtime_error("gen-fail");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== handwritten_generator ===\n";

    std::vector<int> xs;
    for (int x : range(1, 5)) {
        xs.push_back(x);
    }
    assert((xs == std::vector<int>{1, 2, 3, 4}));
    std::cout << "  range(1,5): 1 2 3 4\n";

    // 原理对照:
    //   begin()/++ → handle.resume()  (迭代器 = 同步调度器)
    //   *it        → promise.value_
    //   it==end    → handle.done()
    //   ~Generator → handle.destroy()

    [[maybe_unused]] bool caught = false;
    try {
        for (int x : failing()) {
            assert(x == 1);
            (void)x;
        }
    } catch (const std::runtime_error& e) {
        caught = true;
        std::cout << "  exception via unhandled_exception: " << e.what() << '\n';
    }
    assert(caught);

    std::cout << "handwritten_generator: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section06/handwritten_generator", run>;

}  // namespace
