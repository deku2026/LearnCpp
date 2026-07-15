// Topic     : 递归 lambda（deducing this）
// Doc       : 第2部分-阶段4 · 步骤 5.4 / 阶段3 回链
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: lambda 第一参数 this auto self 表示闭包自身，可递归；无需 y_combinator。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [recursive_lambda_via_deducing_this] ===\n";

    // -------------------------------------------------------------------------
    // §入门：斐波那契
    // -------------------------------------------------------------------------
    auto fib = [](this auto self, int n) -> int { return n < 2 ? n : self(n - 1) + self(n - 2); };
    assert(fib(0) == 0);
    assert(fib(1) == 1);
    assert(fib(10) == 55);
    std::cout << "[intro] fib(10)=" << fib(10) << '\n';

    // -------------------------------------------------------------------------
    // §进阶：带状态的递归（捕获）
    // -------------------------------------------------------------------------
    int calls = 0;
    [[maybe_unused]] auto fib_count = [&calls](this auto self, int n) -> int {
        ++calls;
        return n < 2 ? n : self(n - 1) + self(n - 2);
    };
    assert(fib_count(6) == 8);
    assert(calls > 6);
    std::cout << "[advanced] fib_count calls=" << calls << '\n';

    // -------------------------------------------------------------------------
    // §专家：DFS / 与 std::function 对照
    // -------------------------------------------------------------------------
    // 图：0→1,0→2,1→2
    std::vector<std::vector<int>> g{{1, 2}, {2}, {}};
    std::vector<int> order;
    std::vector<char> seen(g.size(), 0);
    auto dfs = [&](this auto self, int u) -> void {
        if (seen[u]) {
            return;
        }
        seen[u] = 1;
        order.push_back(u);
        for (int v : g[u]) {
            self(v);
        }
    };
    dfs(0);
    assert(order.size() == 3);
    assert(order[0] == 0);

    // 旧法：std::function 自引用（有分配/间接开销）
    std::function<int(int)> fib_f;
    fib_f = [&](int n) -> int { return n < 2 ? n : fib_f(n - 1) + fib_f(n - 2); };
    assert(fib_f(8) == 21);

    std::cout << "[expert] dfs order size=" << order.size() << " fib_f(8)=21\n";
    std::cout << "=== recursive_lambda_via_deducing_this: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section04/recursive_lambda_via_deducing_this", run>;

}  // namespace
