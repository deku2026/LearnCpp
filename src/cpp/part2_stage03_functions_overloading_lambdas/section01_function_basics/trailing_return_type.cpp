// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section01_function_basics
// Item     : trailing_return_type
// Topic id : part2/stage03/section01/trailing_return_type
//
// Covers: auto f() -> T; return type depending on parameters

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>
#include <utility>

namespace {

auto add_plain(int a, int b) -> int {
    return a + b;
}

template <class L, class R>
auto add_dep(const L& lhs, const R& rhs) -> decltype(lhs + rhs) {
    return lhs + rhs;
}

auto make_fp() -> int (*)(int) {
    return [](int x) -> int { return x * 2; };
}

void demo_basics() {
    assert(add_plain(2, 3) == 5);
    static_assert(std::is_same_v<decltype(add_plain(1, 1)), int>);
}

void demo_intermediate() {
    assert(add_dep(1, 2) == 3);
    assert(add_dep(1.5, 2) == 3.5);
    static_assert(std::is_same_v<decltype(add_dep(1, 2.0)), double>);
}

void demo_expert() {
    auto* fp = make_fp();
    assert(fp(21) == 42);

    // Trailing form keeps the function name visible before a complex type.
    auto id = [](int x) -> int { return x; };
    assert(id(7) == 7);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/trailing_return_type", run>;

}  // namespace
