// Topic     : 基本算术运算符重载
// Doc       : 第2部分-阶段4 · 步骤 6.1
// cppreference: https://en.cppreference.com/cpp/language/operators
//
// 要点: 修改左操作数的（+=）宜成员；对称二元（+）宜非成员；+ 可用 += 实现。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>

namespace {

class Vec2 {
    double x_ = 0;
    double y_ = 0;

public:
    Vec2() = default;
    Vec2(double x, double y) : x_(x), y_(y) {}

    double x() const { return x_; }
    double y() const { return y_; }

    Vec2& operator+=(const Vec2& rhs) {
        x_ += rhs.x_;
        y_ += rhs.y_;
        return *this;
    }

    Vec2& operator-=(const Vec2& rhs) {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        return *this;
    }

    // 一元
    Vec2 operator-() const { return {-x_, -y_}; }
};

// 非成员对称 +
Vec2 operator+(Vec2 lhs, const Vec2& rhs) {
    lhs += rhs;
    return lhs;
}

Vec2 operator-(Vec2 lhs, const Vec2& rhs) {
    lhs -= rhs;
    return lhs;
}

// 标量乘法：非成员，允许 2 * v 与 v * 2
Vec2 operator*(Vec2 v, double k) {
    return {v.x() * k, v.y() * k};
}
Vec2 operator*(double k, Vec2 v) {
    return v * k;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [basic_arithmetic_operators] ===\n";

    // -------------------------------------------------------------------------
    // §入门：+ 与 +=
    // -------------------------------------------------------------------------
    Vec2 a{1, 2}, b{3, 4};
    Vec2 c = a + b;
    assert(c.x() == 4 && c.y() == 6);
    a += b;
    assert(a.x() == 4 && a.y() == 6);
    std::cout << "[intro] a+b=(" << c.x() << "," << c.y() << ")\n";

    // -------------------------------------------------------------------------
    // §进阶：一元减、标量乘
    // -------------------------------------------------------------------------
    [[maybe_unused]] Vec2 d = -b;
    assert(d.x() == -3 && d.y() == -4);
    Vec2 e = b * 2.0;
    [[maybe_unused]] Vec2 f = 0.5 * b;
    assert(e.x() == 6 && f.y() == 2);
    std::cout << "[advanced] scalar mul e.x=" << e.x() << '\n';

    // -------------------------------------------------------------------------
    // §专家：成员 vs 非成员选择
    // -------------------------------------------------------------------------
    // 若 + 只做成员：double 无法作为左操作数触发隐式转换到 Vec2（左操作数不转换成员调用）
    // 非成员 + 两边都可参与转换（若有非 explicit 单参构造时）
    Vec2 g = (a - b) + Vec2{0, 0};
    assert(g.x() == 1 && g.y() == 2);
    std::cout << "[expert] (a-b)=(" << g.x() << "," << g.y() << ")\n";
    std::cout << "=== basic_arithmetic_operators: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/basic_arithmetic_operators", run>;

}  // namespace
