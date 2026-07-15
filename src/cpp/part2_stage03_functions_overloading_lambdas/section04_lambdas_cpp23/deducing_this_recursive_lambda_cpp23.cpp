// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section04_lambdas_cpp23
// Item     : deducing_this_recursive_lambda_cpp23
// Topic id : part2/stage03/section04/deducing_this_recursive_lambda_cpp23
//
// Covers: C++23 explicit object parameter recursive lambda; std::function fallback

#include "learn/topic_registry.hpp"

#include <functional>

namespace {

void demo_basics() {
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    auto fib = [](this auto self, int n) -> int { return n < 2 ? n : self(n - 1) + self(n - 2); };
    LEARN_CHECK(fib(10) == 55);
#else
    std::function<int(int)> fib = [&](int n) -> int { return n < 2 ? n : fib(n - 1) + fib(n - 2); };
    LEARN_CHECK(fib(10) == 55);
#endif
}

void demo_intermediate() {
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    auto fact = [](this auto self, int n) -> int { return n <= 1 ? 1 : n * self(n - 1); };
    LEARN_CHECK(fact(5) == 120);
#else
    std::function<int(int)> fact = [&](int n) -> int { return n <= 1 ? 1 : n * fact(n - 1); };
    LEARN_CHECK(fact(5) == 120);
#endif
}

void demo_expert() {
    // Pre-C++23 style still works; has type-erasure cost.
    std::function<int(int)> via_fn = [&](int n) -> int { return n < 2 ? n : via_fn(n - 1) + via_fn(n - 2); };
    LEARN_CHECK(via_fn(7) == 13);

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    auto sum_to = [](this auto self, int n) -> int { return n <= 0 ? 0 : n + self(n - 1); };
    LEARN_CHECK(sum_to(4) == 10);
#else
    LEARN_CHECK(via_fn(4) == 3);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/deducing_this_recursive_lambda_cpp23", run>;

}  // namespace
