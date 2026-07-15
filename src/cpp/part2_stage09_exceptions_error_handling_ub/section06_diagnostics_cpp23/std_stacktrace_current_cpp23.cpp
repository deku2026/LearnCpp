// Topic    : std::stacktrace::current() 抓调用栈（C++23 验收）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 8.1
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section06_diagnostics_cpp23
// Item     : std_stacktrace_current_cpp23
// Topic id : part2/stage09/section06/std_stacktrace_current_cpp23
// Refs     : https://en.cppreference.com/w/cpp/utility/basic_stacktrace
//            P0881
//            ISO [stacktrace]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

// MSVC STL / 较新 libstdc++ 提供 <stacktrace>
#if defined(__has_include)
#if __has_include(<stacktrace>)
#include <stacktrace>
#define LEARN_HAS_STACKTRACE 1
#endif
#endif
#ifndef LEARN_HAS_STACKTRACE
#define LEARN_HAS_STACKTRACE 0
#endif

namespace {

#if LEARN_HAS_STACKTRACE
void log_error_with_trace(const std::string& msg) {
    std::cerr << "error: " << msg << '\n';
    const auto st = std::stacktrace::current();
    std::cerr << "stacktrace (" << st.size() << " frames):\n";
    std::cerr << st << '\n';

    // 逐帧（若实现提供 description/source_file）
    std::size_t shown = 0;
    for (const auto& frame : st) {
        std::cerr << "  " << frame << '\n';
        if (++shown >= 8) {
            break;
        }
    }
    assert(st.size() > 0);
}

void deep_function() {
    log_error_with_trace("something went wrong");
}
void middle_function() {
    deep_function();
}
#endif

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_stacktrace_current_cpp23] 主干 ===\n";
#if LEARN_HAS_STACKTRACE
    {
        middle_function();
        auto st = std::stacktrace::current();
        std::ostringstream oss;
        oss << st;
        const auto text = oss.str();
        assert(!text.empty() || st.empty());  // 极端裁剪环境可能无符号
        std::cout << "captured frames=" << st.size() << '\n';
        std::cout << "note: need debug info (-g /Zi) for file:line\n";
        std::cout << "GCC old libstdc++ may need -lstdc++exp / libbacktrace\n";
    }
#else
    std::cout << "<stacktrace> not available on this toolchain; skipped runtime capture\n";
    std::cout << "see doc step 8.1 for clang-cl/MSVC and GCC link notes\n";
#endif

    std::cout << "=== 专节：与错误处理配合 ===\n";
    // 在 expected 错误分支或 catch 里打印 stacktrace，极大提升可诊断性
    std::cout << "log stacktrace on error paths (expected/catch)\n";

    std::cout << "[std_stacktrace_current_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section06/std_stacktrace_current_cpp23", run>;

}  // namespace
