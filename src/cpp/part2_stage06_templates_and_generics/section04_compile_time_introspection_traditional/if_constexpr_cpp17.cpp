// Topic    : if constexpr —— 编译期分支，丢弃未选中分支
// Doc      : 第2部分-阶段6 · 步骤 10.1–10.2
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : if_constexpr_cpp17
// Topic id : part2/stage06/section04/if_constexpr_cpp17
// Refs     : https://en.cppreference.com/w/cpp/language/if
//            ISO [stmt.if]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 一个函数内按类型分支
// ---------------------------------------------------------------------------

template <typename T>
std::string describe(const T& x) {
    if constexpr (std::is_integral_v<T>) {
        return "integral:" + std::to_string(x);
    } else if constexpr (std::is_floating_point_v<T>) {
        return "floating";
    } else {
        return "other";
    }
}

// ---------------------------------------------------------------------------
// §进阶 — 对比 SFINAE 双写；丢弃分支可含「对当前 T 非法」代码
// ---------------------------------------------------------------------------

// SFINAE 版 half（两个重载）
template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
auto half_sfinae(T x) {
    return x / 2;
}
template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
auto half_sfinae(T x) {
    return x / 2.0;
}

// if constexpr 版（一个函数）
template <typename T>
auto half_cxpr(T x) {
    if constexpr (std::is_integral_v<T>) {
        return x / 2;
    } else {
        return x / 2.0;
    }
}

template <typename T>
auto first_or_self(T& x) {
    if constexpr (requires { x[0]; }) {
        return x[0];
    } else {
        return x;
    }
}

// ---------------------------------------------------------------------------
// §专家 — 递归基线；与 if consteval 的分工
// ---------------------------------------------------------------------------

template <typename T>
constexpr auto recurse_sum(const T& x) {
    if constexpr (std::is_arithmetic_v<T>) {
        return x;
    } else {
        // 容器：对元素递归求和（演示丢弃非法分支）
        using U = std::decay_t<decltype(*x.begin())>;
        U acc{};
        for (const auto& e : x) {
            acc = static_cast<U>(acc + recurse_sum(e));
        }
        return acc;
    }
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [if_constexpr] 入门 ===\n";
    assert(describe(42) == "integral:42");
    assert(describe(3.14) == "floating");
    assert(describe(std::string{"hi"}) == "other");
    std::cout << describe(42) << '\n';

    std::cout << "=== 进阶：SFINAE 对比 + 非法分支丢弃 ===\n";
    assert(half_sfinae(7) == 3);
    assert(std::abs(half_sfinae(7.0) - 3.5) < 1e-12);
    assert(half_cxpr(7) == 3);
    assert(std::abs(half_cxpr(7.0) - 3.5) < 1e-12);

    std::vector<int> v{9, 8, 7};
    assert(first_or_self(v) == 9);
    int n = 5;
    assert(first_or_self(n) == 5);
    // 对 int 实例化时，x[0] 分支被丢弃，不会报错
    std::cout << "half + first_or_self OK\n";

    std::cout << "=== 专家：递归与职责边界 ===\n";
    assert(recurse_sum(3) == 3);
    std::vector<int> nums{1, 2, 3, 4};
    assert(recurse_sum(nums) == 10);
    // if constexpr：按「编译期常量条件」选分支（traits/concepts）
    // if consteval：按「当前是否在常量求值」选分支（见下一 topic / 阶段 7）
    std::cout << "=== if_constexpr_cpp17: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/if_constexpr_cpp17", run>;

}  // namespace
