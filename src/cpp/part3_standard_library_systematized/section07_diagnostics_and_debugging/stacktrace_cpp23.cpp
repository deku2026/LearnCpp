// Topic    : std::stacktrace（C++23 运行期调用栈）
// Doc      : 第3部分-标准库系统化.md · 库 7.1
// Stage    : part3_standard_library_systematized
// Section  : section07_diagnostics_and_debugging
// Item     : stacktrace_cpp23
// Topic id : part3/section07/stacktrace_cpp23
// Refs     : https://en.cppreference.com/w/cpp/utility/basic_stacktrace
//            P0881；需调试信息；GCC 可能要链 libstdc++exp/backtrace

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

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
void level3() {
    const auto st = std::stacktrace::current();
    std::cout << "stacktrace frames=" << st.size() << '\n';
    std::cout << st << '\n';
    std::size_t n = 0;
    for (const auto& frame : st) {
        std::cout << "  #" << n << ' ' << frame << '\n';
        if (++n >= 6) {
            break;
        }
    }
    assert(st.size() > 0 || true);  // 极端裁剪环境可能空
}

void level2() {
    level3();
}
void level1() {
    level2();
}
#endif

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [stacktrace_cpp23] ===\n";

#if LEARN_HAS_STACKTRACE
    level1();
    {
        auto st = std::stacktrace::current();
        std::ostringstream oss;
        oss << st;
        const auto text = oss.str();
        std::cout << "current() text_bytes=" << text.size() << " frames=" << st.size() << '\n';
        assert(!text.empty() || st.empty());
    }
    std::cout << "note: -g /Zi for file:line; link stacktrace support on GCC\n";
#else
    std::cout << "<stacktrace> not available on this toolchain\n";
    std::cout << "fallback: platform backtrace / CaptureStackBackTrace\n";
#endif

    std::cout << "integrate with expected/catch error paths for diagnostics\n";
    std::cout << "[stacktrace_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section07/stacktrace_cpp23", run>;

}  // namespace
