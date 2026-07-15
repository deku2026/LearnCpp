// LearnCpp topic example
// Doc      : part2-stage12-coroutines.md
// Stage    : part2_stage12_coroutines
// Section  : section05_std_generator_cpp23
// Item     : std_generator_as_view
// Topic id : part2/stage12/section05/std_generator_as_view
//
// Covers: std::generator as ranges view (gated)

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <numeric>
#include <utility>
#include <vector>

#if defined(__has_include)
#if __has_include(<generator>)
#include <generator>
#endif
#endif
namespace {

void demo_basics() {
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L && __has_include(<generator>)
    auto g = []() -> std::generator<int> {
        for (int i = 1; i <= 4; ++i) {
            co_yield i;
        }
    }();
    std::vector<int> v;
    for (int x : g) {
        v.push_back(x);
    }
    LEARN_CHECK(v.size() == 4);
#else
    std::vector<int> v{1, 2, 3, 4};
    LEARN_CHECK(v.size() == 4);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L && __has_include(<generator>)
    auto g = []() -> std::generator<int> {
        co_yield 2;
        co_yield 4;
        co_yield 6;
    }();
    int sum = 0;
    for (int x : g) {
        sum += x;
    }
    LEARN_CHECK(sum == 12);
#else
    LEARN_CHECK(std::accumulate(std::vector<int>{2, 4, 6}.begin(), std::vector<int>{2, 4, 6}.end(), 0) == 12);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L && __has_include(<generator>)
    // generator is an input view: single-pass consumption.
    auto g = []() -> std::generator<int> {
        co_yield 1;
        co_yield 2;
    }();
    int n = 0;
    for (int x : g) {
        (void)x;
        ++n;
    }
    LEARN_CHECK(n == 2);
#else
    LEARN_CHECK(true);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section05/std_generator_as_view", run>;

}  // namespace
