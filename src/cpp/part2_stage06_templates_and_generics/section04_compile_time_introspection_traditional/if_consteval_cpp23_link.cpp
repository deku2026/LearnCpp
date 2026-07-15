// Topic    : if consteval（C++23）—— 链接到阶段 7 的编译期/运行期分路
// Doc      : 第2部分-阶段6 · 步骤 10.3（完整展开见阶段 7）
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : if_consteval_cpp23_link
// Topic id : part2/stage06/section04/if_consteval_cpp23_link
// Refs     : https://en.cppreference.com/w/cpp/language/if
//            https://wg21.link/P1938R3
//            ISO [stmt.if]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>

namespace {

// ---------------------------------------------------------------------------
// §入门 — if consteval：是否正在常量求值
// ---------------------------------------------------------------------------

constexpr int square_path(int x) {
    if consteval {
        // 编译期路径：可用纯算法
        return x * x;
    } else {
        // 运行期路径：同样正确；真实代码可换 SIMD / 查表等
        return x * x;
    }
}

// ---------------------------------------------------------------------------
// §进阶 — 与 if constexpr / is_constant_evaluated 对照
// ---------------------------------------------------------------------------

constexpr int demo_is_constant_evaluated(int x) {
    if (std::is_constant_evaluated()) {
        return x + 1;  // 编译期
    }
    return x + 2;  // 运行期
}

// if constexpr 选的是「类型/常量条件」，与求值上下文无关
template <typename T>
constexpr int trait_branch(T x) {
    if constexpr (std::is_integral_v<T>) {
        return static_cast<int>(x) * 10;
    } else {
        return 0;
    }
}

// ---------------------------------------------------------------------------
// §专家 — 编译期与运行期结果可不同（有意设计时）
// ---------------------------------------------------------------------------

constexpr int path_id() {
    if consteval {
        return 1;  // constant-evaluated
    } else {
        return 2;  // runtime
    }
}

consteval int must_be_compile_time(int x) {
    return square_path(x);  // 强制走编译期
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [if_consteval] 入门：双路径同一函数 ===\n";
    constexpr int cx = square_path(5);
    static_assert(cx == 25);
    int rt = square_path(6);
    assert(rt == 36);
    std::cout << "square_path compile=" << cx << " runtime=" << rt << '\n';

    std::cout << "=== 进阶：三种机制对照 ===\n";
    constexpr int a = demo_is_constant_evaluated(10);
    static_assert(a == 11);
    [[maybe_unused]] int b = demo_is_constant_evaluated(10);
    assert(b == 12);

    static_assert(trait_branch(3) == 30);
    static_assert(trait_branch(1.5) == 0);
    // if constexpr：看 T 的性质
    // is_constant_evaluated：表达式上下文（C++20）
    // if consteval：语言级「是否常量求值」（C++23，更清晰）
    std::cout << "trait vs consteval vs is_constant_evaluated OK\n";

    std::cout << "=== 专家：path_id 与 consteval 入口 ===\n";
    constexpr int p1 = path_id();
    static_assert(p1 == 1);
    int p2 = path_id();
    assert(p2 == 2);
    static_assert(must_be_compile_time(4) == 16);
    std::cout << "path_id compile=" << p1 << " runtime=" << p2 << '\n';
    std::cout << "→ 深挖见 stage07 compile-time programming\n";
    std::cout << "=== if_consteval_cpp23_link: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/if_consteval_cpp23_link", run>;

}  // namespace
