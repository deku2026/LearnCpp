// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : std_bind_front_cpp20
// Topic id : part2/stage03/section05/std_bind_front_cpp20
//
// Covers: std::bind_front partial application (leading args)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <string>

namespace {

int sub(int a, int b) {
    return a - b;
}

int add3(int a, int b, int c) {
    return a + b + c;
}

void demo_basics() {
    auto sub10 = std::bind_front(sub, 10);
    assert(sub10(3) == 7);
}

void demo_intermediate() {
    auto add1 = std::bind_front(add3, 1);
    assert(add1(2, 3) == 6);

    auto add1_2 = std::bind_front(add3, 1, 2);
    assert(add1_2(3) == 6);
}

void demo_expert() {
    auto greeter = [](const std::string& g, const std::string& name) { return g + ", " + name; };
    auto hi = std::bind_front(greeter, std::string{"Hi"});
    assert(hi(std::string{"Ada"}) == "Hi, Ada");

    // Equivalent lambda is often clearer for non-trivial logic.
    auto sub10_lam = [](int b) { return sub(10, b); };
    assert(sub10_lam(3) == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_bind_front_cpp20", run>;

}  // namespace
