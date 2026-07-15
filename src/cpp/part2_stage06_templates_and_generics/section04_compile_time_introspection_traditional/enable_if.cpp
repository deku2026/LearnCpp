// Topic    : enable_if —— 把 bool 条件变成 SFINAE 开关
// Doc      : 第2部分-阶段6 · 步骤 9.3
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : enable_if
// Topic id : part2/stage06/section04/enable_if
// Refs     : https://en.cppreference.com/w/cpp/types/enable_if
//            ISO [meta.trans.other]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — enable_if 原理：false 时没有 ::type
// ---------------------------------------------------------------------------

template <bool B, typename T = void>
struct my_enable_if {};

template <typename T>
struct my_enable_if<true, T> {
    using type = T;
};

template <bool B, typename T = void>
using my_enable_if_t = typename my_enable_if<B, T>::type;

// ---------------------------------------------------------------------------
// §进阶 — 三种放置位置：默认模板参数 / 返回类型 / 函数参数
// ---------------------------------------------------------------------------

// ① 默认模板参数
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
T twice_integral(T x) {
    return static_cast<T>(x * 2);
}

// ② 返回类型
template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, T> twice_float(T x) {
    return x * 2;
}

// ③ 函数参数（多一个匿名指针默认实参）
template <typename T>
T negate(T x, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0) {
    return -x;
}

// 互斥重载：整型整除 vs 浮点除
template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
auto half(T x) {
    return x / 2;
}

template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
auto half(T x) {
    return x / 2.0;
}

// ---------------------------------------------------------------------------
// §专家 — 类模板成员启用；与 void_t/concepts 对照
// ---------------------------------------------------------------------------

template <typename T>
class NumericPipe {
public:
    template <typename U = T, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
    U process(U x) const {
        return x + static_cast<U>(1);
    }
};

template <typename T>
my_enable_if_t<std::is_pointer_v<T>, std::remove_pointer_t<T>> load(T p) {
    return *p;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [enable_if] 入门：原理 ===\n";
    static_assert(std::is_same_v<my_enable_if_t<true, int>, int>);
    // my_enable_if_t<false, int> 非法 → 在签名里触发 SFINAE
    std::cout << "my_enable_if true branch has type\n";

    std::cout << "=== 进阶：三种位置与互斥 half ===\n";
    assert(twice_integral(21) == 42);
    assert(std::abs(twice_float(1.5) - 3.0) < 1e-12);
    assert(negate(5) == -5);
    assert(half(7) == 3);
    assert(std::abs(half(7.0) - 3.5) < 1e-12);
    // twice_integral(1.0); // 无匹配
    std::cout << "half(7)=" << half(7) << " half(7.0)=" << half(7.0) << '\n';

    std::cout << "=== 专家：成员启用 + 指针 load ===\n";
    NumericPipe<int> pipe;
    assert(pipe.process(10) == 11);
    int v = 99;
    int* p = &v;
    assert(load(p) == 99);
    // 现代代码优先 Concepts / if constexpr；enable_if 仍大量存在于旧库
    std::cout << "=== enable_if: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/enable_if", run>;

}  // namespace
