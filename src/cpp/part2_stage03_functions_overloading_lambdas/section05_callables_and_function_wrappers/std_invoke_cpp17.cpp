// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : std_invoke_cpp17
// Topic id : part2/stage03/section05/std_invoke_cpp17
//
// Covers: std::invoke for free fn, member ptr, lambda

#include "learn/topic_registry.hpp"

#include <functional>

namespace {

int free_fn(int x) {
    return x * 2;
}

struct S {
    int mem = 42;
    int method(int x) const { return x + mem; }
};

void demo_basics() {
    LEARN_CHECK(std::invoke(free_fn, 5) == 10);
    auto lam = [](int a, int b) { return a + b; };
    LEARN_CHECK(std::invoke(lam, 1, 2) == 3);
}

void demo_intermediate() {
    S s;
    LEARN_CHECK(std::invoke(&S::method, s, 8) == 50);
    LEARN_CHECK(std::invoke(&S::mem, s) == 42);

    S* ps = &s;
    LEARN_CHECK(std::invoke(&S::method, ps, 1) == 43);
}

void demo_expert() {
    // Uniform call syntax for generic wrappers.
    auto call = [](auto&& f, auto&&... args) {
        return std::invoke(std::forward<decltype(f)>(f), std::forward<decltype(args)>(args)...);
    };
    LEARN_CHECK(call(free_fn, 3) == 6);
    S s{10};
    LEARN_CHECK(call(&S::method, s, 5) == 15);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_invoke_cpp17", run>;

}  // namespace
