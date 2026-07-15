// Topic     : 转换运算符
// Doc       : 第2部分-阶段4 · 步骤 6.4
// cppreference: https://en.cppreference.com/cpp/language/cast_operator
//
// 要点: operator T() 定义到 T 的转换；可隐式参与重载决议；易形成意外转换链。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

class Fraction {
    int num_ = 0;
    int den_ = 1;

public:
    Fraction(int n, int d) : num_(n), den_(d) { assert(d != 0); }

    // 非 explicit：允许隐式 Fraction → double
    operator double() const { return static_cast<double>(num_) / static_cast<double>(den_); }

    int num() const { return num_; }
    int den() const { return den_; }
};

struct Meter {
    int v = 0;
    operator int() const { return v; }
};

void take_double(double x) {
    assert(x > 0.0);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [conversion_operators] ===\n";

    // -------------------------------------------------------------------------
    // §入门：隐式转 double
    // -------------------------------------------------------------------------
    Fraction half{1, 2};
    double d = half;  // 隐式
    assert(d == 0.5);
    take_double(half);
    std::cout << "[intro] Fraction→double " << d << '\n';

    // -------------------------------------------------------------------------
    // §进阶：在算术中使用
    // -------------------------------------------------------------------------
    double x = Fraction{3, 4} + 0.25;  // 转换后相加
    assert(x == 1.0);
    Meter m{5};
    int n = m + 2;
    assert(n == 7);
    std::cout << "[advanced] arithmetic via conversion x=" << x << " n=" << n << '\n';

    // -------------------------------------------------------------------------
    // §专家：转换链风险（动机：下题 explicit）
    // -------------------------------------------------------------------------
    // 多个转换运算符 / 构造函数可导致歧义或意外重载匹配。
    // 经验：转换运算符优先 explicit；需要隐式时只保留非常窄的一种。
    std::string s = std::to_string(static_cast<double>(half));
    assert(s.find('5') != std::string::npos || s.find("0.5") != std::string::npos);

    std::cout << "[expert] prefer explicit conversions in real APIs\n";
    std::cout << "=== conversion_operators: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/conversion_operators", run>;

}  // namespace
