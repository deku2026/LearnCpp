// Topic     : 花括号初始化的窄化检查
// Doc       : 第2部分-阶段2 · 步骤 2.2（section01 侧预热）
// cppreference: https://en.cppreference.com/cpp/language/list_initialization
//               https://en.cppreference.com/cpp/language/implicit_conversion  (narrowing)
//
// 要点: list-initialization 禁止窄化；= 与 () 允许（可能仅警告）；
//       显式 static_cast 表达“我知道在截断”。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <climits>
#include <iostream>
#include <type_traits>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [narrowing_check_with_braces] ===\n";

    // -------------------------------------------------------------------------
    // 入门：三种写法对照
    // -------------------------------------------------------------------------
    // 下列“允许窄化”的写法在 -Wnarrowing/-Wconversion 下会警告，但是合法 C++
    int a = static_cast<int>(3.14);                  // copy-init：截断为 3
    [[maybe_unused]] int b(static_cast<int>(3.14));  // direct-init：截断为 3
    // int c{3.14}; // ❌ list-init：narrowing from double to int
    // int d = {3.14}; // ❌ 同样禁止

    assert(a == 3 && b == 3);

    // 范围内的常量可以
    [[maybe_unused]] int ok{42};
    [[maybe_unused]] char ch{'A'};
    // char bad{300}; // ❌ 300 装不进 char（实现 char 位宽下）
    assert(ok == 42 && ch == 'A');
    std::cout << "[intro] a=b=" << a << " from 3.14 via = or (); braces reject it\n";

    // -------------------------------------------------------------------------
    // 进阶：什么算窄化（常见几类）
    // -------------------------------------------------------------------------
    // 1) 浮点 → 整数
    // 2) long double → double/float，或 double → float（常量可能因可精确表示而放行）
    // 3) 整数 → 更小整数，且源不是可表示的常量表达式
    // 4) 整数 → 浮点但无法精确表示（非常量时）

    const int exact = 100;
    [[maybe_unused]] double d1{exact};  // OK：整数常量到 double 精确
    assert(d1 == 100.0);

    [[maybe_unused]] unsigned u{42};  // OK
    // unsigned uneg{-1}; // ❌ 负常量到 unsigned 视为窄化

    // 变量间：即使运行时值“刚好合适”，列表初始化仍可能因类型对而判窄化
    double pi = 3.14159;
    // int truncated{pi}; // ❌
    [[maybe_unused]] int truncated = static_cast<int>(pi);  // 显式：意图清晰
    assert(truncated == 3);

    // 花括号 + 显式 cast：仍是 list-init，但源类型已是 int
    [[maybe_unused]] int explicit_brace{static_cast<int>(pi)};
    assert(explicit_brace == 3);
    std::cout << "[advanced] use static_cast when truncation is intentional\n";

    // -------------------------------------------------------------------------
    // 专家：常量表达式例外与 bool/枚举
    // -------------------------------------------------------------------------
    // 若整数常量表达式的值能在目标类型中表示，则不构成窄化
    const int small = 7;
    [[maybe_unused]] char c2{small};  // OK：7 可表示
    assert(c2 == 7);

    // bool 从整数：非 0/1 的常量到 bool 是窄化
    [[maybe_unused]] bool t{true};
    [[maybe_unused]] bool f{0};  // 0/1 可
    // bool weird{2}; // ❌
    assert(t && !f);

    // 与 auto 组合见 auto_with_initialization；与 vector 的 {} vs () 见 section02
    std::cout << "[expert] narrowing is a compile-time rule on type/value categories\n";
    std::cout << "=== narrowing_check_with_braces: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/narrowing_check_with_braces", run>;

}  // namespace
