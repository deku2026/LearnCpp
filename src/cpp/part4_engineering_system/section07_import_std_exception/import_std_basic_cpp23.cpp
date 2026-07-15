// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.1.6 / section07 import std 基础
// Stage    : part4_engineering_system
// Section  : section07_import_std_exception
// Item     : import_std_basic_cpp23
// Topic id : part4/section07/import_std_basic_cpp23
//
// 要点: import std; 一次导入标准库模块(C++23 方向);
//       工程上仍实验性。本文件用 headers 演示等价能力 + 宏探测。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <version>

namespace {

bool language_modules_reported() {
#ifdef __cpp_modules
    return __cpp_modules != 0;
#else
    return false;
#endif
}

// 「import std 风格」API 使用面 — 实际仍是头文件实现
int demo_std_surface() {
    std::vector<std::string> v{"import", "std", "future"};
    assert(v.size() == 3);
    int n = 0;
    for (const auto& s : v) {
        n += static_cast<int>(s.size());
    }
    return n;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== import_std_basic_cpp23 ===\n";

    std::cout << "  __cpp_modules reported=" << std::boolalpha << language_modules_reported() << '\n';
#ifdef __cpp_modules
    std::cout << "  __cpp_modules=" << __cpp_modules << '\n';
#endif

    // 真正的 `import std;` 需要:
    // - 编译器支持 std 模块
    // - CMake 3.30+ CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
    // - 通常 Ninja
    const bool using_header_fallback = true;
    assert(using_header_fallback);
    assert(demo_std_surface() == 6 + 3 + 6);

    std::cout << "  this TU: classic includes (portable default)\n";
    std::cout << "  when ready: import std; // replaces many #includes\n";
    std::cout << "import_std_basic_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section07/import_std_basic_cpp23", run>;

}  // namespace
