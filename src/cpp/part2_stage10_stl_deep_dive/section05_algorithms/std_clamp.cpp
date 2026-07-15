// Topic     : std::clamp —— 把值夹到 [lo, hi]
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 5.5
// Stage     : part2_stage10_stl_deep_dive
// Section   : section05_algorithms
// Item      : std_clamp
// Topic id  : part2/stage10/section05/std_clamp
// Refs      : https://en.cppreference.com/w/cpp/algorithm/clamp
//             ISO [alg.clamp]

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_clamp] 入门：区间内 / 下溢 / 上溢 ===\n";
    {
        assert(std::clamp(5, 0, 10) == 5);
        assert(std::clamp(-3, 0, 10) == 0);
        assert(std::clamp(99, 0, 10) == 10);
        // 边界等于 lo/hi 时返回边界
        assert(std::clamp(0, 0, 10) == 0);
        assert(std::clamp(10, 0, 10) == 10);
        std::cout << "in-range / below / above OK\n";
    }

    std::cout << "=== 进阶：自定义比较 + 浮点 + 批量夹紧 ===\n";
    {
        const int a = -8;
        [[maybe_unused]] const int b = std::clamp(a, -5, 5);
        assert(b == -5);

        [[maybe_unused]] const double x = std::clamp(1.5, 0.0, 1.0);
        assert(x == 1.0);
        assert(std::clamp(-0.1, 0.0, 1.0) == 0.0);

        // 自定义比较：按绝对值夹到 [1, 5]（教学）
        [[maybe_unused]] const int v = -10;
        [[maybe_unused]] const int lo = -5, hi = 5;
        // 默认 less：-10 < -5 → 返回 lo
        assert(std::clamp(v, lo, hi) == -5);

        std::vector<int> scores{-20, 40, 150, 80};
        for (int& s : scores) s = std::clamp(s, 0, 100);
        assert((scores == std::vector<int>{0, 40, 100, 80}));
        std::cout << "custom range + float + batch clamp OK\n";
    }

    std::cout << "=== 专家：返回 const T& 的悬垂陷阱 + lo<=hi 前置条件 ===\n";
    {
        // ⚠️ clamp 返回 const T& —— 若实参是临时，绑定引用会悬垂
        // const int& bad = std::clamp(1 + 2, 0, 1); // 危险：可能悬垂
        // 安全：按值接收
        [[maybe_unused]] const int safe = std::clamp(1 + 2, 0, 1);
        assert(safe == 1);

        // 具名 lvalue：引用安全
        const int x = 42, lo = 0, hi = 100;
        [[maybe_unused]] const int& r = std::clamp(x, lo, hi);
        assert(&r == &x);  // 在区间内时返回对 x 的引用

        // ⚠️ 前置条件 lo <= hi（用默认 less）；违反则 UB
        // std::clamp(5, 10, 0); // 禁止

        [[maybe_unused]] auto volume = [](int v) { return std::clamp(v, 0, 100); };
        assert(volume(-10) == 0 && volume(50) == 50 && volume(150) == 100);
        std::cout << "const T& lifetime + lo<=hi precondition OK\n";
    }

    std::cout << "[std_clamp] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_clamp", run>;

}  // namespace
