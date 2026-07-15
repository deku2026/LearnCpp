// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section01_function_basics
// Item     : auto_return_type_deduction_cpp14
// Topic id : part2/stage03/section01/auto_return_type_deduction_cpp14
//
// Covers: auto return deduction, auto& vs decltype(auto)

#include "learn/topic_registry.hpp"

#include <type_traits>
#include <utility>

namespace {

auto add(int a, int b) {
    return a + b;
}

auto& pick_ref(int& a, int& b, bool first) {
    return first ? a : b;
}

int g = 10;

decltype(auto) forward_g() {
    return (g);  // parenthesized lvalue -> int&
}

auto copy_g() {
    return (g);  // auto decays: int
}

template <class F, class... Args>
decltype(auto) invoke_and_forward(F&& f, Args&&... args) {
    return std::forward<F>(f)(std::forward<Args>(args)...);
}

int& identity_ref(int& x) {
    return x;
}

void demo_basics() {
    LEARN_CHECK(add(4, 5) == 9);
    static_assert(std::is_same_v<decltype(add(1, 2)), int>);
}

void demo_intermediate() {
    int a = 1;
    int b = 2;
    pick_ref(a, b, true) = 9;
    LEARN_CHECK(a == 9);
    static_assert(std::is_same_v<decltype(pick_ref(a, b, true)), int&>);
}

void demo_expert() {
    g = 10;
    forward_g() = 20;
    LEARN_CHECK(g == 20);
    static_assert(std::is_same_v<decltype(forward_g()), int&>);
    static_assert(std::is_same_v<decltype(copy_g()), int>);
    LEARN_CHECK(copy_g() == 20);

    int x = 3;
    int& r = invoke_and_forward(identity_ref, x);
    r = 8;
    LEARN_CHECK(x == 8);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/auto_return_type_deduction_cpp14", run>;

}  // namespace
