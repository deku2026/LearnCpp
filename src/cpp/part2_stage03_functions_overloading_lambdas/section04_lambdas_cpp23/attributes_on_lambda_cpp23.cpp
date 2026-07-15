// Topic     : lambda 上的属性（C++23）
// Doc       : 第2部分-阶段3 · 步骤 6.2
// cppreference: https://en.cppreference.com/cpp/language/lambda
//               https://en.cppreference.com/cpp/language/attributes
//
// 要点: 可对 lambda 的 operator() 施加 [[nodiscard]] / [[deprecated]] 等；
//       属性位置在参数列表之后说明符区域（与标准语法一致）；
//       与 mutable / constexpr / noexcept / static 可组合。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [attributes_on_lambda_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：[[nodiscard]] 提醒勿丢返回值
    // -------------------------------------------------------------------------
    auto make_id = [] [[nodiscard]] () -> int { return 42; };
    [[maybe_unused]] const int id = make_id();  // 使用返回值
    assert(id == 42);
    // (void)make_id(); 或直接丢弃返回值时，编译器可能告警
    std::cout << "[intro] [[nodiscard]] on lambda call operator\n";

    // -------------------------------------------------------------------------
    // §进阶：与其他说明符组合
    // -------------------------------------------------------------------------
    auto sq = [] [[nodiscard]] (int x) constexpr noexcept { return x * x; };
    static_assert(sq(4) == 16);
    assert(sq(5) == 25);

    int n = 1;
    [[maybe_unused]] auto bump = [n] [[nodiscard]] () mutable { return ++n; };
    assert(bump() == 2);
    assert(bump() == 3);

    // C++23：无参时可省 ()，属性仍贴在说明符区
    auto one = [] [[nodiscard]] constexpr { return 1; };
    static_assert(one() == 1);

    // static + 属性（无捕获）
    [[maybe_unused]] auto twice = [] [[nodiscard]] (int x) static { return x * 2; };
    assert(twice(21) == 42);
    std::cout << "[advanced] attributes + constexpr/noexcept/mutable/static\n";

    // -------------------------------------------------------------------------
    // §专家：deprecated、语法位置、类型擦除边界
    // -------------------------------------------------------------------------
    [[maybe_unused]] auto legacy = [] [[deprecated("use new_api")]] () { return 0; };
    // 调用可能产生弃用警告——本文件刻意调用一次以保留可运行路径
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
    assert(legacy() == 0);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

    // 属性写在 lambda 声明符上，作用于生成的调用运算符（不是捕获列表本身）
    // 无标准属性「强制内联」；[[nodiscard]] 与函数上同语义，实现可忽略未知属性。
    // 经 std::function 类型擦除后，nodiscard 通常不会穿透包装边界。
    [[maybe_unused]] auto raw = [] [[nodiscard]] (int x) { return x + 1; };
    assert(raw(1) == 2);

    // 与尾置返回并用
    [[maybe_unused]] auto unit = [] [[nodiscard]] () -> int { return 1; };
    assert(unit() == 1);

    std::cout << "[expert] lambda attributes annotate the call operator (P1102-era syntax)\n";
    std::cout << "=== attributes_on_lambda_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/attributes_on_lambda_cpp23", run>;

}  // namespace
