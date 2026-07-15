// Topic     : std::clamp —— 把值夹到 [lo, hi]
// Doc       : 第2部分-阶段10 · 步骤 5.5
// cppreference: https://en.cppreference.com/cpp/algorithm/clamp
//
// 要点: C++17; 要求 lo <= hi; 返回 const T& (注意临时悬垂!)。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_clamp] ===\n";

    assert(std::clamp(5, 0, 10) == 5);
    assert(std::clamp(-3, 0, 10) == 0);
    assert(std::clamp(99, 0, 10) == 10);

    // 自定义比较(如按绝对值)
    const int a = -8;
    const int b = std::clamp(a, -5, 5);
    assert(b == -5);

    // 浮点
    const double x = std::clamp(1.5, 0.0, 1.0);
    assert(x == 1.0);

    // ⚠️ clamp 返回 const T& —— 绑定临时会悬垂:
    // const int& bad = std::clamp(1+2, 0, 1); // 可能悬垂(取决于实现)
    // 安全: 按值接收
    const int safe = std::clamp(1 + 2, 0, 1);
    assert(safe == 1);

    // 用于 UI 进度 / 音量等
    auto volume = [](int v) { return std::clamp(v, 0, 100); };
    assert(volume(-10) == 0 && volume(50) == 50 && volume(150) == 100);

    std::cout << "[clamp] in-range/below/above + value-return safety OK\n";
    std::cout << "std_clamp: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_clamp", run>;

}  // namespace
