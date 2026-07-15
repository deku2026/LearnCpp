// Topic     : 字面值类型与 constexpr 构造
// Doc       : 第2部分-阶段4 · 步骤 8.5
// cppreference: https://en.cppreference.com/cpp/language/constant_expression
//
// 要点: constexpr 构造 → 可在核心常量表达式中创建对象；阶段 7 预告；static_assert 验证。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>

namespace {

struct Point {
    int x;
    int y;
    constexpr Point(int x_, int y_) : x(x_), y(y_) {}
    constexpr int manhattan() const {
        // 绝对值简化：教学用非负
        return (x < 0 ? -x : x) + (y < 0 ? -y : y);
    }
};

struct Color {
    unsigned r = 0, g = 0, b = 0;
    constexpr Color() = default;
    constexpr Color(unsigned r_, unsigned g_, unsigned b_) : r(r_), g(g_), b(b_) {}
    constexpr unsigned sum() const { return r + g + b; }
};

constexpr Point midpoint(const Point& a, const Point& b) {
    return Point{(a.x + b.x) / 2, (a.y + b.y) / 2};
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [literal_type_constexpr_constructor] ===\n";

    // -------------------------------------------------------------------------
    // §入门：编译期对象
    // -------------------------------------------------------------------------
    constexpr Point origin{0, 0};
    static_assert(origin.x == 0);
    static_assert(origin.manhattan() == 0);
    std::cout << "[intro] origin.manhattan=" << origin.manhattan() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：constexpr 函数返回类对象
    // -------------------------------------------------------------------------
    constexpr Point a{2, 4};
    constexpr Point b{6, 8};
    constexpr Point m = midpoint(a, b);
    static_assert(m.x == 4 && m.y == 6);
    assert(m.manhattan() == 10);
    std::cout << "[advanced] midpoint=(" << m.x << "," << m.y << ")\n";

    // -------------------------------------------------------------------------
    // §专家：运行期与编译期同一接口
    // -------------------------------------------------------------------------
    int rx = 1, ry = 2;  // 运行期
    Point r{rx, ry};
    assert(r.manhattan() == 3);

    constexpr Color cyan{0, 255, 255};
    static_assert(cyan.sum() == 510);

    // 字面值类型要求：足够「简单」的析构、constexpr 构造等（标准细则阶段 7）
    std::cout << "[expert] runtime Point + constexpr Color sum=" << cyan.sum() << '\n';
    std::cout << "=== literal_type_constexpr_constructor: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section06/literal_type_constexpr_constructor", run>;

}  // namespace
