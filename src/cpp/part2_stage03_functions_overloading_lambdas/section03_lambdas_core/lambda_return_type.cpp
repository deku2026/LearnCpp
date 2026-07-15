// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : lambda_return_type
// Topic id : part2/stage03/section03/lambda_return_type
//
// Covers: deduced vs trailing return; multi-return consistency

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

void demo_basics() {
    auto a = [] { return 42; };
    auto b = [](int x) { return x * 2; };
    auto c = [](int x) -> double { return x / 2.0; };
    assert(a() == 42);
    assert(b(3) == 6);
    assert(c(5) == 2.5);
}

void demo_intermediate() {
    auto choose = [](bool flag) -> int {
        if (flag) {
            return 1;
        }
        return 0;
    };
    assert(choose(true) == 1);
    assert(choose(false) == 0);
    static_assert(std::is_same_v<decltype(choose(true)), int>);
}

void demo_expert() {
    // Explicit trailing return forces a common type across branches.
    auto scale = [](bool as_double, int n) -> double {
        if (as_double) {
            return n * 1.5;
        }
        return static_cast<double>(n);
    };
    assert(scale(true, 2) == 3.0);
    assert(scale(false, 2) == 2.0);

    auto deduced_fn = [](int n) { return n; };
    static_assert(std::is_same_v<decltype(deduced_fn(1)), int>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/lambda_return_type", run>;

}  // namespace
