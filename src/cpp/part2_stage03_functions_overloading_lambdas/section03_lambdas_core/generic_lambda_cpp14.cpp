// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : generic_lambda_cpp14
// Topic id : part2/stage03/section03/generic_lambda_cpp14
//
// Covers: auto parameters -> templated operator()

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>

namespace {

void demo_basics() {
    auto add = [](auto a, auto b) { return a + b; };
    assert(add(1, 2) == 3);
    assert(add(1.5, 2.5) == 4.0);
}

void demo_intermediate() {
    auto size_of = [](const auto& x) { return sizeof(x); };
    assert(size_of(int{}) == sizeof(int));
    assert(size_of(std::string{}) >= 0);

    auto same = [](auto a, auto b) { return std::is_same_v<decltype(a), decltype(b)>; };
    assert(same(1, 2));
    assert(!same(1, 2.0));
}

void demo_expert() {
    // Each auto is an independent template parameter.
    auto first = [](auto a, auto) { return a; };
    assert(first(10, "ignored") == 10);

    auto apply = [](auto f, auto x) { return f(x); };
    assert(apply([](int n) { return n * n; }, 5) == 25);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/generic_lambda_cpp14", run>;

}  // namespace
