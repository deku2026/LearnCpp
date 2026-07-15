// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section02_variadic_functions
// Item     : variadic_templates_intro
// Topic id : part2/stage03/section02/variadic_templates_intro
//
// Covers: parameter packs, sizeof..., C++17 fold expressions (preview)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>

namespace {

template <class... Args>
constexpr std::size_t arity(const Args&...) {
    return sizeof...(Args);
}

template <class... Args>
constexpr int sum_ints(Args... args) {
    return (0 + ... + static_cast<int>(args));
}

template <class... Args>
std::string join_dash(const Args&... args) {
    std::string out;
    ((out += (out.empty() ? "" : "-"), out += args), ...);
    return out;
}

void demo_basics() {
    assert(arity() == 0);
    assert(arity(1, 2, 3) == 3);
    assert(sum_ints(1, 2, 3) == 6);
}

void demo_intermediate() {
    static_assert(sum_ints(4, 5) == 9);
    assert(join_dash(std::string{"a"}, std::string{"b"}, std::string{"c"}) == "a-b-c");
}

void demo_expert() {
    // Mixed types: still type-safe (unlike C varargs).
    assert(arity(1, 2.5, "x") == 3);
    static_assert(std::is_same_v<decltype(sum_ints(1, 2)), int>);
    assert(sum_ints() == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section02/variadic_templates_intro", run>;

}  // namespace
