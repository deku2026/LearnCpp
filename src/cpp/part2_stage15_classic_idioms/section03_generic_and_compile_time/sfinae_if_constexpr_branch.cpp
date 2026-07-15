// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : sfinae_if_constexpr_branch
// Topic id : part2/stage15/section03/sfinae_if_constexpr_branch
//
// Covers: SFINAE vs if constexpr branching

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>
#include <utility>

namespace {

template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
int twice(T x) {
    return static_cast<int>(x) * 2;
}

template <class T>
auto describe(T x) {
    if constexpr (std::is_integral_v<T>) {
        return static_cast<int>(x);
    } else {
        return 0;
    }
}

void demo_basics() {
    assert(twice(5) == 10);
}

void demo_intermediate() {
    assert(describe(7) == 7);
    assert(describe(3.14) == 0);
}

void demo_expert() {
    // Prefer if constexpr / concepts over complex SFINAE when available.
    static_assert(std::is_integral_v<int>);
    assert(twice(static_cast<short>(4)) == 8);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section03/sfinae_if_constexpr_branch", run>;

}  // namespace
