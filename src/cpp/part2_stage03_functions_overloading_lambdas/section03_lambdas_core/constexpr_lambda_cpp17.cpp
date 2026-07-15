// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : constexpr_lambda_cpp17
// Topic id : part2/stage03/section03/constexpr_lambda_cpp17
//
// Covers: constexpr / implicit constexpr operator() on lambdas

#include "learn/topic_registry.hpp"

#include <array>

namespace {

void demo_basics() {
    auto square = [](int n) constexpr { return n * n; };
    static_assert(square(5) == 25);
    LEARN_CHECK(square(6) == 36);
}

void demo_intermediate() {
    // C++17: eligible lambdas are implicitly constexpr.
    auto add = [](int a, int b) { return a + b; };
    static_assert(add(2, 3) == 5);
    constexpr int s = add(4, 5);
    LEARN_CHECK(s == 9);
}

void demo_expert() {
    constexpr auto make = [](int n) { return n * 2; };
    std::array<int, make(3)> arr{};
    static_assert(arr.size() == 6);
    LEARN_CHECK(arr.size() == 6);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/constexpr_lambda_cpp17", run>;

}  // namespace
