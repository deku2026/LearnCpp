// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section02_noexcept_basics
// Item     : noexcept_operator
// Topic id : part2/stage09/section02/noexcept_operator
//
// Covers: noexcept(expr) operator for compile-time checks

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <utility>

namespace {

void no_throw() noexcept {}
void may_throw() {}

struct A {
    A() noexcept = default;
    A(A&&) noexcept = default;
};

struct B {
    B() = default;
    B(B&&) {}  // not noexcept
};

void demo_basics() {
    static_assert(noexcept(no_throw()));
    static_assert(!noexcept(may_throw()));
    LEARN_CHECK(noexcept(no_throw()));
    // ODR-use so -Wunused-function does not fire (noexcept is unevaluated).
    no_throw();
    may_throw();
}

void demo_intermediate() {
    static_assert(std::is_nothrow_move_constructible_v<A>);
    static_assert(!std::is_nothrow_move_constructible_v<B>);
    static_assert(noexcept(A(std::declval<A>())));
}

void demo_expert() {
    // Expression form.
    int x = 1;
    static_assert(noexcept(x + 1));
    std::string s = "hi";
    // string move is typically noexcept
    static_assert(std::is_nothrow_move_constructible_v<std::string>);
    (void)s;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section02/noexcept_operator", run>;

}  // namespace
