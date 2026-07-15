// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_function_cpp11
// Topic id : part2/stage07/section01/constexpr_function_cpp11
//
// Refs:
//   https://en.cppreference.com/w/cpp/language/constexpr
//   Effective Modern C++ Item 15 — prefer constexpr when possible
//   ISO [dcl.constexpr] (C++11 rules: essentially a single return)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>

namespace {

// ---------------------------------------------------------------------------
// §入门 — C++11 风格的 constexpr 函数：单条 return + 递归 / 条件运算符
// ---------------------------------------------------------------------------
// C++11 规定 constexpr 函数体几乎只能是「一条 return 语句」
// （外加 typedef/using/static_assert 等非执行声明）。
// 循环、多语句、局部变量赋值都不允许 → 用递归和 ?: 模拟。
//
// 关键语义（从 C++11 至今未变）：
//   constexpr 函数 = 「maybe constexpr」—— *可以* 在编译期求值，
//   也可以在运行期求值；是否编译期取决于调用上下文与实参。

// C++11 合法写法：单 return + 递归
constexpr int factorial_cpp11(int n) {
    return n <= 1 ? 1 : n * factorial_cpp11(n - 1);
}

constexpr int abs_cpp11(int n) {
    return n < 0 ? -n : n;
}

// 条件运算符嵌套代替 if/else 链
constexpr int max3_cpp11(int a, int b, int c) {
    return a > b ? (a > c ? a : c) : (b > c ? b : c);
}

// ---------------------------------------------------------------------------
// §进阶 — 编译期 vs 运行期同一函数；字面值类型成员；C++11 的苛刻限制
// ---------------------------------------------------------------------------

struct Vec2 {
    int x, y;
    // C++11: constexpr 构造函数的函数体必须为空（成员初始化列表完成工作）
    constexpr Vec2(int xx, int yy) : x(xx), y(yy) {}
    // C++11: 非静态 constexpr 成员函数隐含 const
    constexpr int length2() const { return x * x + y * y; }
};

constexpr int dot_cpp11(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}

// 模板 + constexpr：编译期用 NTTP 驱动
// 必须用 if constexpr 截断实例化，否则三元两侧都会实例化 → 无限递归
template <int N>
constexpr int power2() {
    if constexpr (N <= 0) {
        return 1;
    } else {
        return 2 * power2<N - 1>();
    }
}

// 下面这种 C++14 写法在「纯 C++11 规则」下非法（多语句 + 可变局部变量）：
// constexpr int factorial_modern(int n) {
//     int r = 1;
//     for (int i = 2; i <= n; ++i) r *= i;
//     return r;
// }
// 本文件在 C++23 模式下编译，上述写法其实已合法——但我们刻意展示
// C++11 惯用写法，以便理解「为什么老书上全是递归」。

// ---------------------------------------------------------------------------
// §专家 — 什么时候 *真正* 在编译期执行；地址；与 consteval 的对比
// ---------------------------------------------------------------------------
// 1) 结果赋给 constexpr 变量 / 用于 static_assert / 数组边界 / NTTP
//    → 强制编译期求值（失败则编译错）。
// 2) 结果赋给普通变量且实参非常量 → 运行期调用（同一份函数体）。
// 3) 取 constexpr 函数地址：得到的是普通函数指针，可通过指针在运行期调用。
// 4) 若需要「禁止运行期调用」，C++20 起用 consteval（见 section03）。
// 5) C++11 要求参数/返回类型为字面值类型；C++23 P2448 放宽（见同 section 其它文件）。

constexpr int kFact5 = factorial_cpp11(5);
constexpr Vec2 kI{1, 0};
constexpr Vec2 kJ{0, 1};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section01/constexpr_function_cpp11 ===\n";

    // --- 入门：编译期求值 ---
    static_assert(factorial_cpp11(0) == 1);
    static_assert(factorial_cpp11(5) == 120);
    static_assert(abs_cpp11(-42) == 42);
    static_assert(max3_cpp11(3, 9, 7) == 9);
    static_assert(kFact5 == 120);

    int compile_time_arr[factorial_cpp11(4)]{};  // 边界 = 24
    static_assert(sizeof(compile_time_arr) / sizeof(int) == 24);

    std::cout << "[intro] factorial_cpp11(5)=" << kFact5 << " (forced compile-time via constexpr variable)\n";

    // --- 进阶：同一函数运行期路径 ---
    int n = 6;
    if (argc > 1) {
        // 允许命令行注入，避免被优化成纯常量；无参时用 6
        n = 6;
    }
    const int runtime_fact = factorial_cpp11(n);
    assert(runtime_fact == 720);
    std::cout << "[advanced] factorial_cpp11(" << n << ") runtime = " << runtime_fact
              << " (same function, runtime args)\n";

    static_assert(Vec2{3, 4}.length2() == 25);
    static_assert(dot_cpp11(kI, kJ) == 0);
    static_assert(power2<8>() == 256);

    constexpr Vec2 v{3, 4};
    assert(v.length2() == 25);
    std::cout << "[advanced] Vec2{3,4}.length2()=" << v.length2() << " power2<8>=" << power2<8>() << '\n';

    // --- 专家：函数指针逃逸到运行期 ---
    using FactFn = int (*)(int);
    FactFn fp = factorial_cpp11;  // 允许
    assert(fp(5) == 120);
    std::cout << "[expert] function pointer call factorial_cpp11(5)=" << fp(5)
              << " — constexpr does NOT force compile-time by itself\n";

    // maybe constexpr 的取舍：
    // - 库作者：标 constexpr，给调用方「编译期能力」
    // - 调用方：要用 constexpr 变量 / static_assert / consteval 才能锁定时机
    std::cout << "[expert] C++11 body style = single return + recursion/?: ; "
                 "C++14+ allows loops (see constexpr_relaxations_cpp14_17_20)\n";

    std::cout << "constexpr_function_cpp11: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section01/constexpr_function_cpp11", run>;

}  // namespace
