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
// 要点: 格式化直接输出; println 自动换行; 新代码首选;
//       无 iostream 粘性 manipulator 状态; 可落到 FILE*/ostream。

#include "learn/topic_registry.hpp"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

    // 可断言的“格式化结果”始终用 format(print 本身写终端)
    const auto line = std::format("{} + {} = {}", 1, 2, 3);
    assert(line == "1 + 2 = 3");
    const auto padded = std::format("{:>8}|{:08.2f}|{:#x}", "hi", 3.14159, 255);
    assert(padded.find("hi") != std::string::npos);
    assert(padded.find("0xff") != std::string::npos || padded.find("0xFF") != std::string::npos);
    std::cout << "[intro] format core: " << line << '\n';

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    std::cout << "[intro] __cpp_lib_print=" << __cpp_lib_print << '\n';
    std::print("[intro] {} {}\n", "hello", 42);
    std::println("[intro] auto newline: {}", 3.14);

    // 进阶: FILE* / ostream 重载
    std::print(stdout, "[advanced] via FILE*: {:#x}\n", 255);
    std::ostringstream oss;
    std::print(oss, "streamed={}", 7);
    assert(oss.str() == "streamed=7");

    // 专家: 范围/容器(若实现支持 format ranges)
    std::vector<int> v{1, 2, 3};
    const auto vfmt = std::format("{}", v);
    assert(vfmt.find('1') != std::string::npos);
    std::println("[expert] container format: {}", v);

    std::println(
        "[expert] prefer print/println for formatted output; "
        "iostream remains for stream-state reading");
#elif defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    std::cout << std::format("[intro] {} {}\n", "hello", 42);
    std::cout << std::format("[intro] auto newline: {}\n", 3.14);
    std::cout << "[intro] __cpp_lib_print unavailable; used format+cout\n";
    std::vector<int> v{1, 2, 3};
    std::cout << std::format("[expert] container: {}\n", v);
#else
    std::cout << "[intro] print/format unavailable\n";
#endif

    // 与 iostream 对照: print 无粘性 hex 状态
    std::cout << std::hex << 255 << std::dec << '\n';  // 粘性需手动复位
    assert(std::format("{:d}", 255) == "255");         // format/print 每次独立

    std::cout << "std_print_println_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_print_println_cpp23", run>;

}  // namespace
