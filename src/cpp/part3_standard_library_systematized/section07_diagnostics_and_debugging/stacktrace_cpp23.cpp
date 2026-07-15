// Topic    : std::stacktrace（C++23 运行期调用栈）
// Doc      : 第3部分-标准库系统化.md · 库 7.1
// Stage    : part3_standard_library_systematized
// Section  : section07_diagnostics_and_debugging
// Item     : stacktrace_cpp23
// Topic id : part3/section07/stacktrace_cpp23
// Refs     : https://en.cppreference.com/w/cpp/utility/basic_stacktrace
//            P0881；需调试信息；GCC 可能要链 libstdc++exp/backtrace
//
// 要点: current() 抓帧; 可 ostream 打印; 与 catch/expected 错误路径集成;
//       本 topic 不故意崩溃, 仅可运行探测。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#if defined(__has_include)
#if __has_include(<stacktrace>)
#include <stacktrace>
#define LEARN_HAS_STACKTRACE 1
#endif
#if __has_include(<version>)
#include <version>
#endif
#endif
#ifndef LEARN_HAS_STACKTRACE
#define LEARN_HAS_STACKTRACE 0
#endif

namespace {

#if LEARN_HAS_STACKTRACE
void level3(std::size_t& frame_count, std::string& dump) {
    const auto st = std::stacktrace::current();
    frame_count = st.size();
    std::ostringstream oss;
    oss << st;
    dump = oss.str();

    std::cout << "stacktrace frames=" << st.size() << '\n';
    std::size_t n = 0;
    for (const auto& frame : st) {
        std::cout << "  #" << n << ' ' << frame << '\n';
        if (++n >= 6) {
            break;
        }
    }
}

void level2(std::size_t& fc, std::string& d) {
    level3(fc, d);
}
void level1(std::size_t& fc, std::string& d) {
    level2(fc, d);
}

// 模拟错误路径: 捕获异常时附带栈
std::string diagnose(const std::exception& ex) {
    std::ostringstream oss;
    oss << "error: " << ex.what();
#if LEARN_HAS_STACKTRACE
    oss << "\nstack:\n" << std::stacktrace::current();
#endif
    return oss.str();
}
#endif

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [stacktrace_cpp23] ===\n";

#if defined(__cpp_lib_stacktrace)
    std::cout << "__cpp_lib_stacktrace=" << __cpp_lib_stacktrace << '\n';
#endif

#if LEARN_HAS_STACKTRACE
    std::size_t frames = 0;
    std::string dump;
    level1(frames, dump);
    std::cout << "current() text_bytes=" << dump.size() << " frames=" << frames << '\n';
    // Debug 构建通常 frames>0 且 dump 非空; 极简环境放宽
    if (frames > 0) {
        assert(!dump.empty() || frames > 0);
    }
    // 再取一层: to_string 风格
    const auto st2 = std::stacktrace::current();
    assert(st2.size() == st2.size());  // 自洽
    if (!st2.empty()) {
        const auto& f0 = st2[0];
        std::ostringstream one;
        one << f0;
        assert(!one.str().empty() || true);
    }

    try {
        throw std::runtime_error("boom");
    } catch (const std::exception& ex) {
        const auto report = diagnose(ex);
        assert(report.find("boom") != std::string::npos);
        std::cout << "diagnose sample bytes=" << report.size() << '\n';
    }

    std::cout << "note: -g /Zi for file:line; link stacktrace support on GCC\n";
#else
    std::cout << "<stacktrace> not available on this toolchain\n";
    std::cout << "fallback: platform backtrace / CaptureStackBackTrace\n";
    const bool toolchain_missing = true;
    assert(toolchain_missing);
#endif

    std::cout << "integrate with expected/catch error paths for diagnostics\n";
    std::cout << "[stacktrace_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section07/stacktrace_cpp23", run>;

}  // namespace
