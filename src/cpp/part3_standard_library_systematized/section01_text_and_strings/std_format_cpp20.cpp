// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.4 std::format (C++20)
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_format_cpp20
// Topic id : part3/section01/std_format_cpp20
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/format/format
//   https://en.cppreference.com/w/cpp/utility/format/formatter
//   Feature-test: __cpp_lib_format >= 201907L

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<format>)
#include <format>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/std_format_cpp20 ===\n";

#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    const std::string s = std::format("{} + {} = {}", 1, 2, 3);
    assert(s == "1 + 2 = 3");
    std::cout << "[intro] " << s << '\n';

    // 对齐 / 填充 / 进制 / 浮点
    const std::string line = std::format("{:>8}|{:08.2f}|{:#x}|{:b}", "hi", 3.14159, 255, 10);
    // "      hi|00003.14|0xff|1010"
    assert(line.find("hi") != std::string::npos);
    assert(line.find("0xff") != std::string::npos || line.find("0xFF") != std::string::npos);
    std::cout << "[advanced] " << line << '\n';

    // 位置参数与自动编号
    const std::string pos = std::format("{1} then {0}", "A", "B");
    assert(pos == "B then A");

    // format_to / format_to_n 写迭代器
    std::string out;
    std::format_to(std::back_inserter(out), "x={}", 42);
    assert(out == "x=42");

    std::cout << "[expert] compile-time format string checks when using "
                 "literal; type-safe vs printf\n";
#else
    std::cout << "[intro] <format> / __cpp_lib_format not available\n";
#endif

    std::cout << "std_format_cpp20: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_format_cpp20", run>;

}  // namespace
