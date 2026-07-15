// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section02_if_consteval_cpp23
// Item     : if_consteval_calls_consteval_function
// Topic id : part2/stage07/section02/if_consteval_calls_consteval_function
//
// Covers: calling consteval only from constant context via if consteval

#include "learn/topic_registry.hpp"

#include <cassert>
#include <version>

namespace {

consteval int only_compile_time(int x) {
    return x * x;
}

constexpr int square_smart(int x) {
#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    if consteval {
        return only_compile_time(x);
    }
#endif
    return x * x;  // runtime path (or pre-C++23)
}

void demo_basics() {
    constexpr int a = square_smart(5);
    static_assert(a == 25);
    assert(a == 25);
}

void demo_intermediate() {
    int r = 6;
    assert(square_smart(r) == 36);
}

void demo_expert() {
#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    // consteval function cannot be called at runtime; gated by if consteval.
    constexpr int b = only_compile_time(7);
    static_assert(b == 49);
    assert(b == 49);
#else
    constexpr int b = square_smart(7);
    static_assert(b == 49);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section02/if_consteval_calls_consteval_function", run>;

}  // namespace
