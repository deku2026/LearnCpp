// LearnCpp topic
// Doc      : part2-stage07-compile-time-programming.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section01_constexpr_evolution
// Item     : constexpr_non_literal_signatures_cpp23
// Topic id : part2/stage07/section01/constexpr_non_literal_signatures_cpp23
//
// Refs:
//   P2448R2 — Relaxing some constexpr restrictions
//   https://en.cppreference.com/w/cpp/language/constexpr
//   https://wg21.link/P2448R2
//   Feature-test: __cpp_constexpr >= 202207L (relaxing restrictions)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 问题：C++20 前 constexpr 函数的参数/返回必须是字面值类型
// ---------------------------------------------------------------------------
// 即便函数「只在运行期」用到非字面值类型，签名里出现它也会让整个函数
// 不能标 constexpr。标准库想「全面 constexpr 化」时，被迫写条件宏：
//   #if __cpp_lib_xxx
//   constexpr
//   #endif
// P2448 (C++23) 移除「参数/返回必须是 LiteralType」的硬性诊断要求。

// 非字面值类型：用户定义析构 → 不是字面值类型
struct NonLiteral {
    std::string name;
    NonLiteral() = default;
    explicit NonLiteral(std::string n) : name(std::move(n)) {}
    ~NonLiteral() {}  // 非平凡析构 → 非字面值类型
};

// C++23: 签名里可以出现 NonLiteral
#if defined(__cpp_constexpr) && __cpp_constexpr >= 202207L

constexpr void clear_name(NonLiteral& obj) {
    // 这段逻辑若在常量求值中执行，会因非字面值操作失败；
    // 但函数 *声明* 本身在 C++23 下合法。
    obj.name.clear();
}

constexpr NonLiteral make_runtime_label(int code) {
    // 返回非字面值类型：C++23 允许声明；常量求值调用仍会失败。
    return NonLiteral{"code-" + std::to_string(code)};
}

// 混合：字面值参数路径可真正在编译期求值
constexpr int literal_path(int x) {
    return x * x;
}

// 带非字面值参数的 constexpr 函数 + 字面值重载协作
constexpr int describe_size(const NonLiteral& obj) {
    // size() 在运行期 OK；若强行 constexpr 求值整个调用，
    // 因 NonLiteral 非字面值通常无法完成常量求值。
    return static_cast<int>(obj.name.size());
}

#else

// 旧编译器降级：仅演示字面值路径，并打印说明
constexpr int literal_path(int x) {
    return x * x;
}

#endif

// ---------------------------------------------------------------------------
// §进阶 — 为什么这对标准库 / 泛型 API 重要
// ---------------------------------------------------------------------------
// 1) 允许「一律标 constexpr」而不必拆签名。
// 2) 常量求值是否成功，改由 *调用* 是否满足核心常量表达式决定，
//    而不是在声明期因签名就拒绝。
// 3) 与「maybe constexpr」哲学一致：能力标记 ≠ 每次调用都编译期。

template <class T>
constexpr void touch(T& value) {
    // 泛型里 T 可能是非字面值；C++23 下函数仍可标 constexpr
    (void)value;
}

// ---------------------------------------------------------------------------
// §专家 — 边界：声明合法 ≠ 可 static_assert 调用
// ---------------------------------------------------------------------------
// - constexpr 函数只要「声明合法」即可；
// - 若调用出现在需要常量表达式的上下文，仍必须满足 [expr.const]；
// - 非字面值对象的构造/析构/成员访问经常使常量求值失败；
// - 同时 P2448 还允许「永不满足常量求值」的 constexpr 函数（见下一文件）。

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== section01/constexpr_non_literal_signatures_cpp23 ===\n";

    static_assert(literal_path(7) == 49);
    std::cout << "[intro] literal_path(7)=" << literal_path(7) << " (classic constexpr still works)\n";

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202207L
    std::cout << "[intro] __cpp_constexpr=" << __cpp_constexpr
              << " (>=202207): non-literal signatures accepted (P2448)\n";

    NonLiteral obj{"compile-time-prog"};
    clear_name(obj);  // 运行期调用
    assert(obj.name.empty());

    NonLiteral rebuilt = make_runtime_label(42);
    assert(rebuilt.name == "code-42");
    assert(describe_size(rebuilt) == 7);  // "code-42"

    // 下面若取消注释，通常编译失败：无法在常量表达式中物化 NonLiteral
    // constexpr NonLiteral bad = make_runtime_label(1);
    // static_assert(describe_size(NonLiteral{"x"}) == 1);

    std::cout << "[advanced] runtime clear/make/describe on NonLiteral OK; "
                 "compile-time evaluation of those calls is not required\n";

    std::vector<int> scratch{1, 2, 3};
    touch(scratch);
    std::cout << "[expert] template constexpr touch<vector> is well-formed under "
                 "P2448 even though vector is not a literal type\n";
#else
    std::cout << "[fallback] compiler lacks P2448-level __cpp_constexpr; "
                 "only literal_path demo runs\n";
#endif

    std::cout << "constexpr_non_literal_signatures_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section01/constexpr_non_literal_signatures_cpp23", run>;

}  // namespace
