// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库4 · 4.3 <numbers>
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : numbers_constants
// Topic id : part3/section04/numbers_constants
//
// Refs:
//   https://en.cppreference.com/w/cpp/numeric/constants
//
// 要点: C++20 数学常量; *_v<T> 模板版本; 替代 M_PI 等非标准宏。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <numbers>
#include <type_traits>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section04/numbers_constants ===\n";

    using std::numbers::e;
    using std::numbers::egamma;
    using std::numbers::inv_pi;
    using std::numbers::inv_sqrt3;
    using std::numbers::ln10;
    using std::numbers::ln2;
    using std::numbers::log10e;
    using std::numbers::log2e;
    using std::numbers::phi;
    using std::numbers::pi;
    using std::numbers::sqrt2;
    using std::numbers::sqrt3;

    // --- 入门: 常用常量范围 ---
    assert(pi > 3.14 && pi < 3.15);
    assert(e > 2.71 && e < 2.72);
    assert(sqrt2 > 1.414 && sqrt2 < 1.415);
    assert(phi > 1.618 && phi < 1.619);
    assert(ln2 > 0.69 && ln2 < 0.70);

    // --- 进阶: 恒等式交叉验证 ---
    assert(std::abs(log2e * ln2 - 1.0) < 1e-12);
    assert(std::abs(inv_pi * pi - 1.0) < 1e-12);
    assert(std::abs(log10e * ln10 - 1.0) < 1e-12);
    assert(std::abs(sqrt3 * inv_sqrt3 - 1.0) < 1e-12);
    assert(egamma > 0.57 && egamma < 0.58);

    // 模板版本: 指定浮点类型
    constexpr auto pif = std::numbers::pi_v<float>;
    constexpr auto pid = std::numbers::pi_v<double>;
    static_assert(pif > 3.0f && pif < 3.2f);
    static_assert(pid > 3.14 && pid < 3.15);
    static_assert(std::is_same_v<decltype(pif), const float>);

    const double circ = 2 * pi * 10.0;
    assert(circ > 62.0 && circ < 63.0);

    // e^{ln2} ≈ 2
    assert(std::abs(std::exp(ln2) - 2.0) < 1e-12);

    std::cout << "[intro] pi=" << pi << " e=" << e << " sqrt2=" << sqrt2 << '\n';
    std::cout << "[advanced] pi_v<float>=" << pif << " circumference(r=10)=" << circ << '\n';
    std::cout << "[expert] prefer numbers::* over M_PI / magic literals (portable)\n";
    std::cout << "numbers_constants: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/numbers_constants", run>;

}  // namespace
