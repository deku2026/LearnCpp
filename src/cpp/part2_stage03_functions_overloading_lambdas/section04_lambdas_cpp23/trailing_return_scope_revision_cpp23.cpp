// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section04_lambdas_cpp23
// Item     : trailing_return_scope_revision_cpp23
// Topic id : part2/stage03/section04/trailing_return_scope_revision_cpp23
//
// Covers: trailing-return / lambda scope notes; portable patterns

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>
#include <utility>

namespace {

template <class L, class R>
auto add_trail(const L& lhs, const R& rhs) -> decltype(lhs + rhs) {
    return lhs + rhs;
}

void demo_basics() {
    auto f = [](int x) -> int { return x + 1; };
    assert(f(1) == 2);
    assert(add_trail(1, 2.5) == 3.5);
}

void demo_intermediate() {
    // Trailing return sees parameters; useful for decltype on args.
    auto mul = [](auto a, auto b) -> decltype(a * b) { return a * b; };
    assert(mul(2, 3) == 6);
    static_assert(std::is_same_v<decltype(mul(2, 3.0)), double>);
}

void demo_expert() {
    // C++23 refinements keep trailing-return usable with modern lambda forms.
    int n = 4;
#if defined(__cplusplus) && __cplusplus >= 202302L
    auto g = [n]() -> int { return n * n; };
#else
    auto g = [n]() -> int { return n * n; };
#endif
    assert(g() == 16);

    auto h = [](int x) noexcept -> int { return x; };
    assert(h(9) == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/trailing_return_scope_revision_cpp23", run>;

}  // namespace
