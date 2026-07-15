// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : recursive_lambda_via_deducing_this_idiom
// Topic id : part2/stage15/section04/recursive_lambda_via_deducing_this_idiom
//
// Covers: recursive lambda via deducing this

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>

namespace {

void demo_basics() {
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    auto fib = [](this auto self, int n) -> int {
        if (n < 2) {
            return n;
        }
        return self(n - 1) + self(n - 2);
    };
    assert(fib(10) == 55);
#else
    std::function<int(int)> fib = [&](int n) -> int {
        if (n < 2) {
            return n;
        }
        return fib(n - 1) + fib(n - 2);
    };
    assert(fib(10) == 55);
#endif
}

void demo_intermediate() {
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    auto fact = [](this auto self, int n) -> int { return n <= 1 ? 1 : n * self(n - 1); };
    assert(fact(5) == 120);
#else
    std::function<int(int)> fact = [&](int n) -> int { return n <= 1 ? 1 : n * fact(n - 1); };
    assert(fact(5) == 120);
#endif
}

void demo_expert() {
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    auto sum = [](this auto self, int n) -> int { return n <= 0 ? 0 : n + self(n - 1); };
    assert(sum(4) == 10);
#else
    std::function<int(int)> sum = [&](int n) -> int { return n <= 0 ? 0 : n + sum(n - 1); };
    assert(sum(4) == 10);
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage15/section04/recursive_lambda_via_deducing_this_idiom", run>;

}  // namespace
