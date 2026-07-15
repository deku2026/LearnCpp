// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section02_if_consteval_cpp23
// Item     : if_consteval_versus_is_constant_evaluated
// Topic id : part2/stage07/section02/if_consteval_versus_is_constant_evaluated
//
// Covers: if consteval vs std::is_constant_evaluated differences

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>
#include <version>

namespace {

constexpr int via_is_constant_evaluated() {
    if (std::is_constant_evaluated()) {
        return 1;
    }
    return 2;
}

constexpr int via_if_consteval() {
#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    if consteval {
        return 1;
    } else {
        return 2;
    }
#else
    return via_is_constant_evaluated();
#endif
}

// Subtle: is_constant_evaluated in a boolean context can be tricky with if constexpr.
constexpr int mixed() {
    if constexpr (true) {
        return via_if_consteval();
    }
    return 0;
}

void demo_basics() {
    constexpr int a = via_is_constant_evaluated();
    static_assert(a == 1);
    assert(via_is_constant_evaluated() == 2);
}

void demo_intermediate() {
    constexpr int b = via_if_consteval();
    static_assert(b == 1);
    assert(via_if_consteval() == 2);
}

void demo_expert() {
    constexpr int c = mixed();
    static_assert(c == 1);
    assert(mixed() == 2);

    // Teaching note: if consteval is the clear C++23 tool for "compile vs run" split;
    // is_constant_evaluated remains useful on C++20.
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage07/section02/if_consteval_versus_is_constant_evaluated", run>;

}  // namespace
