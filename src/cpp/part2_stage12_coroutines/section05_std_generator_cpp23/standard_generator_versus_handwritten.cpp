// LearnCpp topic
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : standard_generator_versus_handwritten
// Topic id : part2/stage12/section05/standard_generator_versus_handwritten
//
// 要点: 对照手写 Generator 与 std::generator — 日常用标准版, 手写为看穿原理。
// Refs:
//   https://en.cppreference.com/w/cpp/coroutine/generator
//   https://en.cppreference.com/w/cpp/language/coroutines

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <exception>
#include <iostream>
#include <utility>
#include <vector>

#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
#include <generator>
#define LEARN_HAS_STD_GENERATOR 1
#else
#define LEARN_HAS_STD_GENERATOR 0
#endif

namespace {

// ---------------------------------------------------------------------------
// 手写最小 Generator(与 section06 同构, 本文件自包含)
// ---------------------------------------------------------------------------
template <typename T>
class HandGen {
public:
    struct promise_type {
        T value_{};
        std::exception_ptr exception_{};

        HandGen get_return_object() { return HandGen{Handle::from_promise(*this)}; }
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
    explicit HandGen(Handle h) noexcept : handle_(h) {}
    ~HandGen() {
        if (handle_) {
            handle_.destroy();
        }
    }
    HandGen(const HandGen&) = delete;
    HandGen(HandGen&& o) noexcept : handle_(std::exchange(o.handle_, {})) {}

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
            handle_.resume();
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

HandGen<int> hand_range(int lo, int hi) {
    for (int i = lo; i < hi; ++i) {
        co_yield i;
    }
}

#if LEARN_HAS_STD_GENERATOR
std::generator<int> std_range(int lo, int hi) {
    for (int i = lo; i < hi; ++i) {
        co_yield i;
    }
}
#endif

template <typename R>
std::vector<int> collect(R&& r) {
    std::vector<int> out;
    for (int x : r) {
        out.push_back(x);
    }
    return out;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== standard_generator_versus_handwritten ===\n";

    const auto hand = collect(hand_range(1, 5));
    assert((hand == std::vector<int>{1, 2, 3, 4}));
    std::cout << "  handwritten: 1 2 3 4\n";

#if LEARN_HAS_STD_GENERATOR
    const auto stdg = collect(std_range(1, 5));
    assert(stdg == hand);
    std::cout << "  std::generator: same sequence\n";
#endif

    std::cout << "  comparison:\n";
    std::cout << "    | concern            | handwritten      | std::generator      |\n";
    std::cout << "    | basic co_yield     | DIY promise+iter | built-in            |\n";
    std::cout << "    | ranges pipeline    | DIY view concept | is a view           |\n";
    std::cout << "    | recursive yield    | O(depth)/elem    | elements_of + xfer  |\n";
    std::cout << "    | exception nesting  | careful DIY      | handled             |\n";
    std::cout << "    | allocator          | DIY              | pmr::generator etc. |\n";
    std::cout << "  rule: use std::generator daily; hand-roll to learn the machinery.\n";

    std::cout << "standard_generator_versus_handwritten: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/standard_generator_versus_handwritten", run>;

}  // namespace
