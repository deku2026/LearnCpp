// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_unreachable_cpp23_recap
// Topic id : part2/stage10/section05_algorithms/std_unreachable_cpp23_recap
//
// Covers: std::unreachable C++23 recap — mark impossible paths (not called)

#include "learn/topic_registry.hpp"

#include <version>

#if defined(__cpp_lib_unreachable) && __cpp_lib_unreachable >= 202202L
#include <utility>
#endif

namespace {

int classify(int x) {
    if (x > 0) {
        return 1;
    }
    if (x < 0) {
        return -1;
    }
    return 0;
}

int only_positive(int x) {
    if (x > 0) {
        return x * 2;
    }
    // x <= 0 is not expected by callers of this helper in the expert demo
#if defined(__cpp_lib_unreachable) && __cpp_lib_unreachable >= 202202L
    // Do not call unreachable on reachable paths — only document the API.
    (void)x;
    return -1;
#else
    return -1;
#endif
}

void demo_basics() {
    LEARN_CHECK(classify(5) == 1);
    LEARN_CHECK(classify(-3) == -1);
    LEARN_CHECK(classify(0) == 0);
}

void demo_intermediate() {
    LEARN_CHECK(only_positive(4) == 8);
    LEARN_CHECK(only_positive(-1) == -1);
}

void demo_expert() {
    // std::unreachable() tells the optimizer a path is impossible.
    // Calling it when reachable is UB — never invoke it in demos that run.
#if defined(__cpp_lib_unreachable) && __cpp_lib_unreachable >= 202202L
    constexpr bool has_unreachable = true;
#else
    constexpr bool has_unreachable = false;
#endif
    LEARN_CHECK(has_unreachable || !has_unreachable);
    LEARN_CHECK(classify(1) != classify(-1));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05_algorithms/std_unreachable_cpp23_recap", run>;

}  // namespace
