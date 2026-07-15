// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §4.6
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : recursive_lambda_via_deducing_this_idiom
// Topic id : part2/stage15/section04/recursive_lambda_via_deducing_this_idiom
//
// Recursive lambda: old std::function / Y-combinator vs C++23 deducing this.
// Motive : local recursive algorithms without naming a free function.
// Modern : [](this auto&& self, ...) { self(...); }
// Pitfall: complex recursion is often clearer as a named function; watch return type
//          deduction (annotate trailing return when needed).

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>

namespace {

// Classic Y-combinator sketch (pre-deducing-this, no std::function heap).
template <typename F>
struct Y {
    F f;
    template <typename... Args>
    decltype(auto) operator()(Args&&... args) const {
        return f(*this, std::forward<Args>(args)...);
    }
};
template <typename F>
Y(F) -> Y<F>;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== old: std::function recursive lambda ==\n";
    std::function<int(int)> fact_old = [&](int n) -> int { return n <= 1 ? 1 : n * fact_old(n - 1); };
    assert(fact_old(5) == 120);
    std::cout << "  fact_old(5)=" << fact_old(5) << " (type erasure cost)\n";

    std::cout << "== old: Y-combinator (no type erasure) ==\n";
    auto fact_y = Y{[](auto self, int n) -> int { return n <= 1 ? 1 : n * self(n - 1); }};
    assert(fact_y(6) == 720);
    std::cout << "  fact_y(6)=" << fact_y(6) << '\n';

    std::cout << "== C++23: deducing this ==\n";
    auto fact = [](this auto&& self, int n) -> int { return n <= 1 ? 1 : n * self(n - 1); };
    assert(fact(5) == 120);
    std::cout << "  fact(5)=" << fact(5) << '\n';

    // Tree walk / DFS style local recursion
    std::vector<std::vector<int>> g(4);
    g[0] = {1, 2};
    g[1] = {3};
    g[2] = {};
    g[3] = {};
    std::vector<int> order;
    auto dfs = [&](this auto&& self, int u) -> void {
        order.push_back(u);
        for (int v : g[u]) {
            self(v);
        }
    };
    dfs(0);
    assert((order == std::vector<int>{0, 1, 3, 2}));
    std::cout << "  dfs order:";
    for (int u : order) {
        std::cout << ' ' << u;
    }
    std::cout << '\n';

    std::cout << "prefer deducing this for small local recursion; name big algorithms\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage15/section04/recursive_lambda_via_deducing_this_idiom", run>;

}  // namespace
