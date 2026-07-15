// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.1.6 / section07 import std 例外
// Stage    : part4_engineering_system
// Section  : section07_import_std_exception
// Item     : import_std_basic_cpp23
// Topic id : part4/section07/import_std_basic_cpp23
//
// 要点: import std; 一次导入标准库模块(C++23 方向);
//       本仓库可移植默认仍 headers; 演示等价 API 面 + 宏探测。

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

// 「import std」后的 API 使用面 — 实际仍用头文件实现
int demo_std_surface() {
    std::vector<std::string> v{"import", "std", "future"};
    assert(v.size() == 3);
    int n = 0;
    for (const auto& s : v) {
        n += static_cast<int>(s.size());
    }
    return n;
}

struct ImportStdReadiness {
    bool cmake_ge_330;
    bool ninja_generator;
    bool experimental_flag;
    bool compiler_std_module;
    bool ready() const { return cmake_ge_330 && ninja_generator && experimental_flag && compiler_std_module; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== import_std_basic_cpp23 ===\n";

    std::cout << "  __cpp_modules reported=" << std::boolalpha << language_modules_reported() << '\n';
#ifdef __cpp_modules
    std::cout << "  __cpp_modules=" << __cpp_modules << '\n';
#endif
#ifdef __cpp_lib_modules
    std::cout << "  __cpp_lib_modules=" << __cpp_lib_modules << '\n';
#endif

    // 真正 `import std;` 需要:
    // - 编译器支持 std 模块
    // - CMake 3.30+ CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
    // - 通常 Ninja
    ImportStdReadiness r{
        /*cmake_ge_330*/ false,  // 仓库基线 3.28
        /*ninja*/ true,          // presets 使用 Ninja
        /*experimental_flag*/ false,
        /*compiler_std_module*/ false,
    };
    assert(!r.ready());
    assert(r.ninja_generator);

    const bool using_header_fallback = true;
    assert(using_header_fallback);
    assert(demo_std_surface() == 6 + 3 + 6);

    // 心智: import std 替换大量 #include <...>, 不替换第三方库
    const int classic_includes_replaced = 3;  // vector/string/iostream 示意
    assert(classic_includes_replaced >= 1);

    std::cout << "  this TU: classic includes (portable default)\n";
    std::cout << "  when ready: import std; // replaces many #includes\n";
    std::cout << "  readiness cmake>=3.30 + Ninja + experimental + toolchain\n";
    std::cout << "import_std_basic_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section07/import_std_basic_cpp23", run>;

}  // namespace
