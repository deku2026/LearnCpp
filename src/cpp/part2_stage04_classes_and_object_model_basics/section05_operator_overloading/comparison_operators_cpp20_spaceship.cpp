// Topic     : 三路比较 <=>（spaceship）
// Doc       : 第2部分-阶段4 · 步骤 7.1–7.3
// cppreference: https://en.cppreference.com/cpp/language/operator_comparison
//
// 要点: a<=>b 得 ordering；a<b 可重写为 (a<=>b)<0；三种 ordering 类别。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <compare>
#include <iostream>
#include <limits>
#include <string>

namespace {

struct Version {
    int major = 0;
    int minor = 0;
    int patch = 0;

    // 手写 <=>（非 default）—— 仅演示返回类型与重写；== 需另写
    std::strong_ordering operator<=>(const Version& rhs) const {
        if (auto c = major <=> rhs.major; c != 0) {
            return c;
        }
        if (auto c = minor <=> rhs.minor; c != 0) {
            return c;
        }
        return patch <=> rhs.patch;
    }

    bool operator==(const Version& rhs) const { return major == rhs.major && minor == rhs.minor && patch == rhs.patch; }
};

struct FloatBox {
    double v = 0.0;
    // double 的 <=> 是 partial_ordering（NaN）
    std::partial_ordering operator<=>(const FloatBox& o) const { return v <=> o.v; }
    bool operator==(const FloatBox& o) const { return v == o.v; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [comparison_operators_cpp20_spaceship] ===\n";

    // -------------------------------------------------------------------------
    // §入门：关系运算由 <=> 重写
    // -------------------------------------------------------------------------
    Version a{1, 2, 0}, b{1, 3, 0};
    assert(a < b);
    assert(a <= b);
    assert(!(a > b));
    assert(a != b);
    assert((a <=> b) < 0);
    std::cout << "[intro] 1.2.0 < 1.3.0\n";

    // -------------------------------------------------------------------------
    // §进阶：strong_ordering 结果
    // -------------------------------------------------------------------------
    Version c{1, 2, 0};
    assert((a <=> c) == std::strong_ordering::equal);
    assert(a == c);
    Version d{2, 0, 0};
    assert((d <=> a) == std::strong_ordering::greater);
    std::cout << "[advanced] strong_ordering equal/greater\n";

    // -------------------------------------------------------------------------
    // §专家：partial_ordering / NaN
    // -------------------------------------------------------------------------
    FloatBox x{1.0}, y{2.0};
    assert(x < y);
    FloatBox nan{std::numeric_limits<double>::quiet_NaN()};
    auto ord = nan <=> x;
    assert(ord == std::partial_ordering::unordered);
    assert(!(nan < x) && !(nan > x) && !(nan == x));

    std::cout << "[expert] NaN unordered under partial_ordering\n";
    std::cout << "=== comparison_operators_cpp20_spaceship: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/comparison_operators_cpp20_spaceship", run>;

}  // namespace
