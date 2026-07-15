// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.4 format 编译期 vs 运行期
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_format_runtime_versus_compile_time_check_cpp23
// Topic id : part3/section01/std_format_runtime_versus_compile_time_check_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/format/basic_format_string
//   https://en.cppreference.com/w/cpp/utility/format/runtime_format
//   P2918R2 / runtime_format (C++26 周边; C++23 用 format_string / vformat)
//
// 要点: 字面量格式串 → 编译期检查参数个数/类型;
//       运行期串 → vformat / runtime 机制, 错误变为运行期异常。

#include "learn/topic_registry.hpp"

#include <cassert>
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
    std::cout << "=== part3/section01/std_format_runtime_versus_compile_time_check_cpp23 ===\n";

#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
    // 编译期检查路径: 格式串是字面量 → basic_format_string 在编译期解析
    const std::string ok = std::format("{} {}", "hello", 42);
    assert(ok == "hello 42");
    std::cout << "[intro] compile-time-checked literal format: " << ok << '\n';

    // 下面若取消注释, 编译失败(参数不足/类型不对)——这是相对 printf 的核心优势:
    // std::format("{} {}", 1);          // 编译错误
    // std::format("{:d}", "not-int");   // 编译错误

    // 运行期格式串: 用 vformat + make_format_args
    // make_format_args 要求左值参数(存引用, 不能绑临时量)
    const std::string runtime_fmt = "{}-{}";
    int a = 1;
    int b = 2;
    const std::string runtime_out = std::vformat(runtime_fmt, std::make_format_args(a, b));
    assert(runtime_out == "1-2");
    std::cout << "[advanced] vformat runtime string -> " << runtime_out << '\n';

    // 非法运行期格式: 抛 format_error
    [[maybe_unused]] bool threw = false;
    try {
        std::string not_int = "x";
        (void)std::vformat("{:d}", std::make_format_args(not_int));
    } catch (const std::format_error& e) {
        threw = true;
        std::cout << "[expert] runtime format_error: " << e.what() << '\n';
    } catch (const std::exception& e) {
        // 部分实现对类型不匹配抛 system_error/其他
        threw = true;
        std::cout << "[expert] runtime exception: " << e.what() << '\n';
    }
    // 非法格式串本身(缺右花括号)在多数实现上抛 format_error
    [[maybe_unused]] bool bad_spec = false;
    try {
        const std::string bad = "{";
        (void)std::vformat(bad, std::make_format_args(a));
    } catch (const std::exception&) {
        bad_spec = true;
    }
    assert(bad_spec || threw);  // 至少一种运行期错误路径被触发
    assert(std::format("{:04d}", 7) == "0007");

    std::cout << "[expert] prefer literal format strings for safety; "
                 "use vformat when the pattern is truly dynamic\n";
#else
    std::cout << "[intro] <format> not available\n";
    assert(false && "format baseline expected on this course toolchain");
#endif

    std::cout << "std_format_runtime_versus_compile_time_check_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part3/section01/std_format_runtime_versus_compile_time_check_cpp23", run>;

}  // namespace
