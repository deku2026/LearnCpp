// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库4 · 4.5 numeric_limits
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : numeric_limits
// Topic id : part3/section04/numeric_limits
//
// Refs:
//   https://en.cppreference.com/w/cpp/types/numeric_limits

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section04/numeric_limits ===\n";

    using lim_i = std::numeric_limits<int>;
    assert(lim_i::is_specialized);
    assert(lim_i::is_integer);
    assert(lim_i::is_signed);
    assert(lim_i::min() < 0);
    assert(lim_i::max() > 0);
    assert(lim_i::lowest() == lim_i::min());  // 整数: lowest==min
    std::cout << "[intro] int min=" << lim_i::min() << " max=" << lim_i::max() << '\n';

    using lim_d = std::numeric_limits<double>;
    assert(lim_d::is_specialized);
    assert(!lim_d::is_integer);
    assert(lim_d::max() > lim_d::lowest());  // lowest 是最负
    assert(lim_d::epsilon() > 0.0);
    assert(lim_d::infinity() > lim_d::max());
    assert(std::isnan(lim_d::quiet_NaN()));
    std::cout << "[advanced] double epsilon=" << lim_d::epsilon() << " digits10=" << lim_d::digits10 << '\n';

    // 浮点比较: 用 epsilon 相对误差思路
    const double a = 0.1 + 0.2;
    const double b = 0.3;
    const double tol = 1e-9;
    assert(std::abs(a - b) < tol);

    // bool / char
    assert(std::numeric_limits<bool>::max() == true);
    std::cout << "[expert] char is_signed=" << std::numeric_limits<char>::is_signed << '\n';

    std::cout << "numeric_limits: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/numeric_limits", run>;

}  // namespace
