// Topic    : std::print / std::println（C++23）在 IO 层的定位
// Doc      : 第3部分-标准库系统化.md · 库 5.1 vs print
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : std_print_cpp23
// Topic id : part3/section05/std_print_cpp23
// Refs     : https://en.cppreference.com/w/cpp/io/print
//            P2093；__cpp_lib_print
//
// 要点: 新代码格式化输出首选 print; iostream 保留流状态/自定义 <<;
//       无粘性 manipulator; 可写 stderr。

#include "learn/topic_registry.hpp"

#include <cstdio>
#include <format>
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

    // format 始终可断言(print 写终端不便 assert)
    const auto line = std::format("[{}] {}", code, where);
    assert(line == "[404] gateway");
    assert(std::format("{:#x}", code) == "0x194");
    std::cout << "format check: " << line << '\n';

#if LEARN_HAS_PRINT
#if defined(__cpp_lib_print)
    std::cout << "__cpp_lib_print=" << __cpp_lib_print << '\n';
#endif
    std::println("error {} at {}", code, where);
    std::print("hex code={:#x}\n", code);
    std::println(stderr, "diag on stderr: {}", where);

    std::ostringstream oss;
    std::print(oss, "{}:{}", where, code);
    assert(oss.str() == "gateway:404");
#else
    std::cout << std::format("error {} at {}\n", code, where);
    std::cout << std::format("hex code={:#x}\n", code);
    std::cerr << std::format("diag on stderr: {}\n", where);
    std::cout << "<print> missing; used format fallback\n";
#endif

    std::cout << "=== 与 iostream 对比要点 ===\n";
    // · print: 无粘性 manipulator；类型安全；常更快
    // · iostream: 流状态机、自定义 <<、与 streambuf 深度集成
    std::vector<int> v{1, 2, 3};
    const auto vline = std::format("{}", v);
    assert(vline.find('1') != std::string::npos);
    std::cout << std::format("container via format: {}\n", v);

    // 粘性状态演示: hex 污染后续输出
    std::cout << std::hex << 255 << ' ' << 10 << std::dec << '\n';
    // print/format 不受此影响
    assert(std::format("{}", 10) == "10");

    std::cout << "[std_print_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/std_print_cpp23", run>;

}  // namespace
