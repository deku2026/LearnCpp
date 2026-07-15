// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section12_coroutine_library
// Item     : std_generator_cpp23_recap
// Topic id : part2/stage10/section12_coroutine_library/std_generator_cpp23_recap
//
// Covers: std::generator C++23 recap — lazy view from coroutine

#include "learn/topic_registry.hpp"

#include <cassert>
#include <ranges>
#include <vector>
#include <version>

#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
#include <generator>
#endif

namespace {

#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
std::generator<int> count_to(int n) {
    for (int i = 1; i <= n; ++i) {
        co_yield i;
    }
}
#endif

void demo_basics() {
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    std::vector<int> out;
    for (int x : count_to(3)) {
        out.push_back(x);
    }
    assert((out == std::vector<int>{1, 2, 3}));
#else
    // Fallback: iota view models the same lazy sequence idea
    auto g = std::views::iota(1, 4);
    std::vector<int> out(g.begin(), g.end());
    assert((out == std::vector<int>{1, 2, 3}));
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    int sum = 0;
    for (int x : count_to(5)) {
        sum += x;
    }
    assert(sum == 15);
#else
    int sum = 0;
    for (int x : std::views::iota(1, 6)) {
        sum += x;
    }
    assert(sum == 15);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    // generator is an input_range / view-like sequence from coroutines
    auto g = count_to(4);
    static_assert(std::ranges::input_range<decltype(g)>);
    assert(std::ranges::distance(count_to(4)) == 4);
#else
    assert(std::ranges::distance(std::views::iota(1, 5)) == 4);
#endif
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section12_coroutine_library/std_generator_cpp23_recap", run>;

}  // namespace
