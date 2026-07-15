// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section04_lambdas_cpp23
// Item     : optional_parameter_list_cpp23
// Topic id : part2/stage03/section04/optional_parameter_list_cpp23
//
// Covers: C++23 omit empty () even with mutable/constexpr/noexcept

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

void demo_basics() {
    auto a = [] { return 1; };
    assert(a() == 1);

    // Pre-C++23 style with empty parameter list still valid.
    auto b = []() { return 2; };
    assert(b() == 2);
}

void demo_intermediate() {
#if defined(__cplusplus) && __cplusplus >= 202302L
    // P1102: empty () optional even with specifiers.
    int n = 0;
    auto c = [n] mutable { return ++n; };
    assert(c() == 1);
    assert(c() == 2);

    auto d = [] constexpr { return 7; };
    static_assert(d() == 7);
#else
    int n = 0;
    auto c = [n]() mutable { return ++n; };
    assert(c() == 1);
    auto d = []() constexpr { return 7; };
    static_assert(d() == 7);
#endif
}

void demo_expert() {
#if defined(__cplusplus) && __cplusplus >= 202302L
    auto e = [] noexcept { return 3; };
    assert(e() == 3);
#else
    auto e = []() noexcept { return 3; };
    assert(e() == 3);
#endif
    // With parameters, () is still required.
    auto f = [](int x) { return x + 1; };
    assert(f(4) == 5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/optional_parameter_list_cpp23", run>;

}  // namespace
