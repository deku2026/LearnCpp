// Topic    : 有符号整数溢出 UB —— 检查与无符号对照
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 11.1 / 11.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : signed_integer_overflow
// Topic id : part2/stage09/section08/signed_integer_overflow
// Refs     : https://en.cppreference.com/w/cpp/language/ub
//            UBSan signed-integer-overflow

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>

namespace {

[[maybe_unused]] std::optional<int> safe_add(int a, int b) {
    // 在更宽类型上检查，避免有符号溢出 UB
    const std::int64_t sum = static_cast<std::int64_t>(a) + static_cast<std::int64_t>(b);
    if (sum < std::numeric_limits<int>::min() || sum > std::numeric_limits<int>::max()) {
        return std::nullopt;
    }
    return static_cast<int>(sum);
}

unsigned defined_wrap(unsigned a, unsigned b) {
    return a + b;  // 无符号溢出良定义（模 2^n）
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [signed_integer_overflow] 主干：安全加法 ===\n";
    {
        assert(safe_add(2, 3) == 5);
        assert(!safe_add(std::numeric_limits<int>::max(), 1));
        assert(!safe_add(std::numeric_limits<int>::min(), -1));
        std::cout << "safe_add rejects overflowing cases\n";
    }

    std::cout << "=== 对抗：无符号环绕（良定义）===\n";
    {
        const unsigned r = defined_wrap(std::numeric_limits<unsigned>::max(), 1u);
        assert(r == 0u);
        std::cout << "unsigned max+1 -> " << r << '\n';
    }

    std::cout << "=== 专节：UB 与错误检查反模式（不触发）===\n";
    // 危险（勿运行）：
    //   int x = INT_MAX; int y = x + 1;           // 有符号溢出 UB
    //   if (x + 1 < x) { /* overflow */ }         // 优化器可删掉
    // 正确：先用更宽类型/无符号/显式饱和检查；开发构建开 UBSan。
    std::cout << "signed overflow is UB; do not check via x+1<x\n";
    std::cout << "clang++ -fsanitize=undefined catches many overflows\n";

    std::cout << "[signed_integer_overflow] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/signed_integer_overflow", run>;

}  // namespace
