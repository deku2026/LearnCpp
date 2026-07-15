// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : std_invoke_r_cpp23
// Topic id : part2/stage03/section05/std_invoke_r_cpp23
//
// Covers: std::invoke_r<R> (C++23) with feature-test gate

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <type_traits>

namespace {

int free_fn(int x) {
    return x * 2;
}

struct S {
    int mem = 42;
    int method(int x) const { return x + mem; }
};

void demo_basics() {
#if defined(__cpp_lib_invoke_r) && __cpp_lib_invoke_r >= 202106L
    const double d = std::invoke_r<double>(free_fn, 5);
    assert(d == 10.0);
#else
    const double d = static_cast<double>(std::invoke(free_fn, 5));
    assert(d == 10.0);
#endif
}

void demo_intermediate() {
    S s;
#if defined(__cpp_lib_invoke_r) && __cpp_lib_invoke_r >= 202106L
    const long r = std::invoke_r<long>(&S::method, s, 8);
    assert(r == 50L);
#else
    const long r = static_cast<long>(std::invoke(&S::method, s, 8));
    assert(r == 50L);
#endif
}

void demo_expert() {
    auto lam = [](int a, int b) { return a + b; };
#if defined(__cpp_lib_invoke_r) && __cpp_lib_invoke_r >= 202106L
    static_assert(std::is_same_v<decltype(std::invoke_r<int>(lam, 1, 2)), int>);
    assert(std::invoke_r<int>(lam, 1, 2) == 3);
    assert(std::invoke_r<double>(lam, 1, 2) == 3.0);
#else
    assert(static_cast<int>(std::invoke(lam, 1, 2)) == 3);
    assert(static_cast<double>(std::invoke(lam, 1, 2)) == 3.0);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_invoke_r_cpp23", run>;

}  // namespace
