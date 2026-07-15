// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.4 std::print / println (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_print_println_cpp23
// Topic id : part3/section01/std_print_println_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/io/print
//   https://en.cppreference.com/w/cpp/io/println
//   Feature-test: __cpp_lib_print >= 202207L
//
// 要点: 格式化直接输出; println 自动换行; 新代码首选, 替代 iostream 拼接。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<print>)
#include <print>
#endif
#if __has_include(<format>)
#include <format>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/std_print_println_cpp23 ===\n";

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    std::print("[intro] {} {}\n", "hello", 42);
    std::println("[intro] auto newline: {}", 3.14);

    // 可输出到 FILE* / ostream 重载(实现提供时)
    std::print(stdout, "[advanced] via FILE*: {:#x}\n", 255);

    std::println(
        "[expert] prefer print/println for formatted output; "
        "iostream remains for stream-state reading");
#elif defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    // 降级: format + cout
    std::cout << std::format("[intro] {} {}\n", "hello", 42);
    std::cout << std::format("[intro] auto newline: {}\n", 3.14);
    std::cout << "[intro] __cpp_lib_print unavailable; used format+cout\n";
#else
    std::cout << "[intro] print/format unavailable\n";
#endif

    std::cout << "std_print_println_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_print_println_cpp23", run>;

}  // namespace
