// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.5.4 std::stacktrace (C++23)
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : stacktrace_in_runtime_cpp23
// Topic id : part4/section05/stacktrace_in_runtime_cpp23
//
// 要点: 运行期捕获调用栈, 增强断言/日志。特性用 __cpp_lib_stacktrace 探测。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <version>

#if defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
#include <stacktrace>
#define LEARN_HAS_STACKTRACE 1
#else
#define LEARN_HAS_STACKTRACE 0
#endif

#if !defined(__cpp_lib_expected) || !(__cpp_lib_expected)
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] std::expected not available (__cpp_lib_expected)\n";
    return 0;
}
[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/stacktrace_in_runtime_cpp23", run>;
}  // namespace
#else
namespace {

std::string current_stack_description() {
#if LEARN_HAS_STACKTRACE
    auto st = std::stacktrace::current();
    if (st.empty()) {
        return "(empty stacktrace — need debug info / platform support)";
    }
    // 取前几帧描述
    std::string out;
    const std::size_t n = std::min<std::size_t>(st.size(), 5);
    for (std::size_t i = 0; i < n; ++i) {
        out += st[i].description();
        out += " | ";
    }
    return out;
#else
    return "(std::stacktrace not available on this toolchain — fallback)";
#endif
}

// 断言失败时附带栈(教学)
void ensure(bool cond, const char* msg) {
    if (!cond) {
        std::cout << "  ASSERT " << msg << "\n  at " << current_stack_description() << '\n';
        assert(cond);
    }
}

int leaf() {
    ensure(true, "leaf ok");
    return 42;
}

[[maybe_unused]] int mid() {
    return leaf();
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== stacktrace_in_runtime_cpp23 ===\n";

#if defined(__cpp_lib_stacktrace)
    std::cout << "  __cpp_lib_stacktrace=" << __cpp_lib_stacktrace << '\n';
#else
    std::cout << "  __cpp_lib_stacktrace undefined\n";
#endif

    assert(mid() == 42);
    auto desc = current_stack_description();
    assert(!desc.empty());
    std::cout << "  sample: " << desc << '\n';

    // 工程用法: 崩溃处理器 / EXPECT 失败日志 / std::expected 错误附加栈
    std::cout << "  use with debug symbols (-g / /Zi) for useful frames\n";
    std::cout << "stacktrace_in_runtime_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/stacktrace_in_runtime_cpp23", run>;

}  // namespace
#endif  // __cpp_lib_expected
