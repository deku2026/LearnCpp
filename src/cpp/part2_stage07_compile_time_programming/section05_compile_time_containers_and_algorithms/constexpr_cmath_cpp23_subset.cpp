// LearnCpp topic example
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_cmath_cpp23_subset
// Topic id : part2/stage07/section05/constexpr_cmath_cpp23_subset
//
// Covers: constexpr <cmath> subset (C++23) with feature tests / portable fallbacks

#include "learn/topic_registry.hpp"

#include <cmath>
#include <version>

namespace {

constexpr int ipow(int b, int e) {
    int r = 1;
    for (int i = 0; i < e; ++i) {
        r *= b;
    }
    return r;
}

void demo_basics() {
    LEARN_CHECK(std::abs(-5) == 5);
    LEARN_CHECK(std::abs(5) == 5);
    static_assert(ipow(2, 8) == 256);
}

void demo_intermediate() {
#if defined(__cpp_lib_constexpr_cmath) && __cpp_lib_constexpr_cmath >= 202202L
    constexpr auto a = std::abs(-3);
    static_assert(a == 3);
    LEARN_CHECK(a == 3);
#else
    // Portable: use our constexpr helper for compile-time, std::abs at runtime.
    constexpr int a = ipow(3, 1);
    static_assert(a == 3);
    LEARN_CHECK(std::abs(-3) == 3);
#endif
}

void demo_expert() {
    // Runtime floating math always available.
    LEARN_CHECK(std::fabs(-2.5) == 2.5);
    LEARN_CHECK(std::floor(3.7) == 3.0);
    LEARN_CHECK(std::ceil(3.2) == 4.0);

#if defined(__cpp_lib_constexpr_cmath)
    static_assert(__cpp_lib_constexpr_cmath >= 0);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_cmath_cpp23_subset", run>;

}  // namespace
