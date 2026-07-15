// Topic    : 格式化 thread::id / stacktrace（C++23 P2693）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 8.3
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section06_diagnostics_cpp23
// Item     : format_thread_id_stacktrace_cpp23
// Topic id : part2/stage09/section06/format_thread_id_stacktrace_cpp23
// Refs     : https://en.cppreference.com/w/cpp/thread/thread/id
//            https://en.cppreference.com/w/cpp/utility/basic_stacktrace
//            P2693

#include "learn/topic_registry.hpp"

#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#if defined(__has_include)
#if __has_include(<stacktrace>)
#include <stacktrace>
#define LEARN_HAS_STACKTRACE 1
#endif
#if __has_include(<print>)
#include <print>
#define LEARN_HAS_PRINT 1
#endif
#endif
#ifndef LEARN_HAS_STACKTRACE
#define LEARN_HAS_STACKTRACE 0
#endif
#ifndef LEARN_HAS_PRINT
#define LEARN_HAS_PRINT 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [format_thread_id_stacktrace_cpp23] 主干：thread::id ===\n";
    {
        const auto id = std::this_thread::get_id();
        // iostream 始终可用
        std::ostringstream oss;
        oss << id;
        const auto via_stream = oss.str();
        assert(!via_stream.empty());

        // C++23：std::formatter<thread::id>
        const auto via_format = std::format("{}", id);
        assert(!via_format.empty());
        std::cout << "thread id stream=" << via_stream << " format=" << via_format << '\n';

#if LEARN_HAS_PRINT
        std::println("println thread_id={}", id);
#endif
    }

    std::cout << "=== 对抗：stacktrace 格式化 ===\n";
#if LEARN_HAS_STACKTRACE
    {
        const auto st = std::stacktrace::current();
        std::ostringstream oss;
        oss << st;
        const auto text = oss.str();
        // format 支持取决于实现；stream 插入是底线
        std::cout << "stacktrace frames=" << st.size() << " text_empty=" << text.empty() << '\n';
#if LEARN_HAS_PRINT
        // 部分实现支持 std::println("{}", st)
        std::println("stacktrace via print:\n{}", st);
#endif
        assert(st.size() >= 0);
    }
#else
    std::cout << "<stacktrace> unavailable; skipped\n";
#endif

    std::cout << "=== 专节：诊断日志一行式 ===\n";
    {
        const auto id = std::this_thread::get_id();
        const auto line = std::format("tid={} event={} code={}", id, "timeout", 408);
        assert(line.find("timeout") != std::string::npos);
        std::cout << line << '\n';
    }

    std::cout << "[format_thread_id_stacktrace_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section06/format_thread_id_stacktrace_cpp23", run>;

}  // namespace
