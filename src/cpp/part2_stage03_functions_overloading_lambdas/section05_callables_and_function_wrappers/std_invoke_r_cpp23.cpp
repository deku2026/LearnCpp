// Topic     : std::invoke_r（C++23）
// Doc       : 第2部分-阶段3 · 步骤 7.2
// cppreference: https://en.cppreference.com/cpp/utility/functional/invoke
//
// 要点: invoke_r<R>(f, args...) 以指定类型 R 返回（转换/void 丢弃）；
//       固定接口返回类型时比手写 static_cast 更清晰。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <type_traits>

namespace {

double scale(int x) {
    return x * 1.5;
}

struct Gen {
    int operator()(int x) const { return x * 10; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_invoke_r_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：指定返回类型
    // -------------------------------------------------------------------------
    // scale 返回 double；invoke_r<int> 转换为 int
    const int n = std::invoke_r<int>(scale, 4);  // 6.0 → 6
    assert(n == 6);
    std::cout << "[intro] invoke_r<R> converts result to R\n";

    // -------------------------------------------------------------------------
    // §进阶：void 与原样类型
    // -------------------------------------------------------------------------
    int side = 0;
    std::invoke_r<void>([&](int v) { side = v; }, 9);  // 丢弃/无返回
    assert(side == 9);

    const int same = std::invoke_r<int>(Gen{}, 3);
    assert(same == 30);

    const double d = std::invoke_r<double>([](int x) { return x; }, 5);
    assert(d == 5.0);
    std::cout << "[advanced] invoke_r<void> discards; same-type works too\n";

    // -------------------------------------------------------------------------
    // §专家：与 invoke / is_invocable_r
    // -------------------------------------------------------------------------
    static_assert(std::is_invocable_r_v<int, decltype(scale), int>);
    static_assert(std::is_invocable_r_v<void, decltype(scale), int>);  // 结果可丢弃

    auto lam = [](int a, int b) { return a + b; };
    assert(std::invoke_r<long long>(lam, 2, 3) == 5LL);

    // 泛型库接口：调用方策略返回「任意」，你用 invoke_r<Expected> 规范出口类型
    auto pipeline = [](auto&& f, auto&&... args) {
        return std::invoke_r<int>(std::forward<decltype(f)>(f), std::forward<decltype(args)>(args)...);
    };
    assert(pipeline([](double x) { return x; }, 3.9) == 3);

    std::cout << "[expert] invoke_r standardizes result conversion at the call boundary\n";
    std::cout << "=== std_invoke_r_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_invoke_r_cpp23", run>;

}  // namespace
