// Topic    : C++23 std::print / std::println — 新 Hello World(验收点)
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 15.1
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_print_println_cpp23
// Topic id : part2/stage10/section10/std_print_println_cpp23
// Refs     : https://en.cppreference.com/w/cpp/io/print
//            https://en.cppreference.com/w/cpp/io/println
//            P2093R14  Feature-test: __cpp_lib_print

#include "learn/topic_registry.hpp"

#include <cstdio>
#include <format>
#include <iostream>
#include <string>
#include <vector>

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
#include <print>
#define LEARN_HAS_PRINT 1
#else
#define LEARN_HAS_PRINT 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_print_println_cpp23] print/println vs iostream ===\n";

    const std::string name = "World";
    const int count = 42;
    const std::vector<int> v{1, 2, 3};

    // 验收: 用 format/print 风格替换 iostream 拼接
    // 旧: cout << "Hello, " << name << "! count = " << count << '\n';
    // 新: println("Hello, {}! count = {}", name, count);
    const std::string joined = std::format("Hello, {}! count = {}", name, count);
    assert(joined == "Hello, World! count = 42");
    std::cout << "  format-style: " << joined << '\n';

#if LEARN_HAS_PRINT
    std::cout << "  __cpp_lib_print=" << __cpp_lib_print << '\n';
    // println 自动换行; print 不自动换行
    std::println("  println Hello, {}!", name);
    std::print("  print count = {}\n", count);

    // 指定 FILE* 目标(stderr)
    std::println(stderr, "  stderr note: count={}", count);

    // 格式化容器(C++23 ranges format, 与 print 组合)
#if defined(__cpp_lib_format_ranges) && __cpp_lib_format_ranges >= 202207L
    std::println("  container: {}", v);
#else
    std::println("  container via format: {}", std::format("{}", v.size()));
#endif
#else
    std::cout << "  (no <print>) fallback iostream + format\n";
    std::cout << "  Hello, " << name << "! count = " << count << '\n';
#endif

    // print ≠ 仅语法糖: 设计目标含 Unicode 终端输出(Windows 上更可靠)
    // 本例用 format 断言证明类型安全格式化语义
    assert(std::format("{:04d}", count) == "0042");
    std::cout << "  zero-padded: " << std::format("{:04d}", count) << '\n';

    std::cout << "[std_print_println_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section10/std_print_println_cpp23", run>;

}  // namespace
