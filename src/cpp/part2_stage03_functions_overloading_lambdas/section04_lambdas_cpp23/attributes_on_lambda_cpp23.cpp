// Topic     : lambda 上的属性（C++23）
// Doc       : 第2部分-阶段3 · 步骤 6.2
// cppreference: https://en.cppreference.com/cpp/language/lambda
//               https://en.cppreference.com/cpp/language/attributes
//
// 要点: 可对 lambda 的 operator() 施加 [[nodiscard]] / [[deprecated]] 等；
//       属性位置在参数列表之后、说明符附近（见标准语法）。

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
    const int id = make_id();  // 使用返回值
    assert(id == 42);
    // (void)make_id(); 若直接丢弃，优质编译器可告警
    std::cout << "[intro] [[nodiscard]] on lambda call operator\n";

    // -------------------------------------------------------------------------
    // §进阶：与其它说明符组合
    // -------------------------------------------------------------------------
    auto sq = [] [[nodiscard]] (int x) constexpr noexcept { return x * x; };
    static_assert(sq(4) == 16);
    assert(sq(5) == 25);

    int n = 1;
    auto bump = [n] [[nodiscard]] () mutable { return ++n; };
    assert(bump() == 2);
    std::cout << "[advanced] attributes + constexpr/noexcept/mutable\n";

    // -------------------------------------------------------------------------
    // §专家：deprecated 与属性语法位置
    // -------------------------------------------------------------------------
    auto legacy = [] [[deprecated("use new_api")]] () { return 0; };
    // 调用可能产生弃用警告——这里调用一次以保持可运行，警告可接受
    assert(legacy() == 0);

    // 属性写在 lambda 声明符上，作用于调用运算符，而非闭包类型本身的一切用途。
    // 无标准属性「强制报错」——[[nodiscard]] 是提示，实现可警告。
    // 若工具链对 lambda 属性支持不完整，用包装函数表达同样意图。
    std::cout << "[expert] lambda attributes annotate the call operator (P1102 era syntax)\n";
    std::cout << "=== attributes_on_lambda_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/attributes_on_lambda_cpp23", run>;

}  // namespace
