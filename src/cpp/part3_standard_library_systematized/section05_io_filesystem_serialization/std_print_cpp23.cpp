// Topic    : std::print / std::println（C++23）在 IO 层的定位
// Doc      : 第3部分-标准库系统化.md · 库 5.1 vs print
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : std_print_cpp23
// Topic id : part3/section05/std_print_cpp23
// Refs     : https://en.cppreference.com/w/cpp/io/print
//            P2093；__cpp_lib_print

#include "learn/topic_registry.hpp"

#include <cassert>
#include <format>
#include <iostream>
#include <string>
#include <vector>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<print>)
#include <print>
#define LEARN_HAS_PRINT 1
#endif
#endif
#ifndef LEARN_HAS_PRINT
#define LEARN_HAS_PRINT 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_print_cpp23] 格式化输出 ===\n";

    const int code = 404;
    const std::string where = "gateway";

#if LEARN_HAS_PRINT
#if defined(__cpp_lib_print)
    std::cout << "__cpp_lib_print=" << __cpp_lib_print << '\n';
#endif
    std::println("error {} at {}", code, where);
    std::print("hex code={:#x}\n", code);
    // 写到 stderr
    std::println(stderr, "diag on stderr: {}", where);
#else
    std::cout << std::format("error {} at {}\n", code, where);
    std::cout << std::format("hex code={:#x}\n", code);
    std::cerr << std::format("diag on stderr: {}\n", where);
    std::cout << "<print> missing; used format fallback\n";
#endif

    // format 始终可断言
    const auto line = std::format("[{}] {}", code, where);
    assert(line == "[404] gateway");
    std::cout << "format check: " << line << '\n';

    std::cout << "=== 与 iostream 对比要点 ===\n";
    // · print: 无粘性 manipulator 状态；类型安全；常更快
    // · iostream: 流状态机、自定义 <<、与 streambuf 深度集成
    std::vector<int> v{1, 2, 3};
    std::cout << std::format("container via format: {}\n", v);

    std::cout << "[std_print_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/std_print_cpp23", run>;

}  // namespace
