// Topic    : std::print / std::println 格式化诊断（C++23）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 8.3
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section06_diagnostics_cpp23
// Item     : std_print_for_diagnostics_cpp23
// Topic id : part2/stage09/section06/std_print_for_diagnostics_cpp23
// Refs     : https://en.cppreference.com/w/cpp/io/print
//            P2093

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

#if defined(__has_include)
#if __has_include(<print>)
#include <print>
#define LEARN_HAS_PRINT 1
#endif
#endif
#ifndef LEARN_HAS_PRINT
#define LEARN_HAS_PRINT 0
#endif

#include <format>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_print_for_diagnostics_cpp23] 主干 ===\n";

    const int code = 404;
    const std::string where = "router";

#if LEARN_HAS_PRINT
    std::println("error {} at {}", code, where);
    std::print("detail: code={:#x} where={}\n", code, where);
    std::cout << "(std::print/println used above)\n";
#else
    // 回退：format + iostream（语义相同，便于旧工具链）
    std::cout << std::format("error {} at {}\n", code, where);
    std::cout << std::format("detail: code={:#x} where={}\n", code, where);
    std::cout << "<print> missing; used std::format fallback\n";
#endif

    // format 始终可测
    const auto line = std::format("[{}] {}", code, where);
    assert(line == "[404] router");
    std::cout << "format check: " << line << '\n';

    std::cout << "=== 专节：诊断场景 ===\n";
    // 类型安全格式化；配合 stacktrace / thread::id（见下一 topic）
    // 比 iostream 拼装更清晰，比 printf 类型安全
    std::cout << "prefer print/format for structured diagnostics\n";

    std::cout << "[std_print_for_diagnostics_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section06/std_print_for_diagnostics_cpp23", run>;

}  // namespace
