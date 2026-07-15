// Topic     : C 风格变参 ...（了解即可，现代代码优先不用）
// Doc       : 第2部分-阶段3 · 步骤 8.3
// cppreference: https://en.cppreference.com/cpp/language/variadic_arguments
//               https://en.cppreference.com/cpp/utility/variadic
//
// 要点: va_list / va_start / va_arg / va_end；无类型安全；类型不符 → UB；
//       现代 C++ 用 initializer_list 或可变参数模板替代；仅对接 C API 时需要。

#include "learn/topic_registry.hpp"

#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <string>

namespace {

// 传统：最后一个具名参数之后是 ...
[[maybe_unused]] int sum_c(int count, ...) {
    va_list ap;
    va_start(ap, count);
    int total = 0;
    for (int i = 0; i < count; ++i) {
        // 调用者必须保证传的是 int；传 double 等 → UB
        total += va_arg(ap, int);
    }
    va_end(ap);
    return total;
}

// 安全得多的现代替代（同类型）
[[maybe_unused]] int sum_modern(std::initializer_list<int> xs) {
    int t = 0;
    for (int x : xs) {
        t += x;
    }
    return t;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [legacy_c_variadic] ===\n";

    // -------------------------------------------------------------------------
    // §入门：最小 va_arg 循环
    // -------------------------------------------------------------------------
    assert(sum_c(3, 10, 20, 30) == 60);
    assert(sum_c(0) == 0);
    std::cout << "[intro] sum_c(count, ...) via va_list works when types match\n";

    // -------------------------------------------------------------------------
    // §进阶：与 printf 族同源的风险
    // -------------------------------------------------------------------------
    // printf("%d", 3.14) 之类格式/实参不匹配是经典 UB。
    // 编译器可对 printf 做格式检查；自定义 ... 函数通常没有这种保护。
    char buf[64]{};
    std::snprintf(buf, sizeof(buf), "n=%d", 42);
    assert(std::string{buf} == "n=42");

    assert(sum_modern({10, 20, 30}) == 60);
    std::cout << "[advanced] prefer type-safe alternatives; snprintf for C-style formatting\n";

    // -------------------------------------------------------------------------
    // §专家：为何几乎不用 + 对接边界
    // -------------------------------------------------------------------------
    // 1) 默认实参提升：float 会提升成 double，short/char 提升成 int——va_arg 类型必须按提升后写。
    // 2) 无法在语言层校验包长度/类型；count 靠调用者自觉。
    // 3) C++ 替代：initializer_list（同类型）、variadic templates + fold（异构）、
    //    format (C++20)、span 等。
    // 4) 仅当包装 C 库（如自定义日志桥到 syslog）时保留 thin wrapper。
    // 5) ⚠️ 本 topic 故意不演示「错误类型的 va_arg」——那是 UB，不能「跑给你看」。

    std::cout << "[expert] C variadic is legacy; use packs/format; never mismatch va_arg types\n";
    std::cout << "=== legacy_c_variadic: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section02/legacy_c_variadic", run>;

}  // namespace
