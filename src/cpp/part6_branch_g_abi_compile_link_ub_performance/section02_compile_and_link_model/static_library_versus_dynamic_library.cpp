// LearnCpp topic
// Doc      : 第6部分-支线G · G5 静态 vs 动态库（验收点）
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : static_library_versus_dynamic_library
// Topic id : part6/g/section02/static_library_versus_dynamic_library
//
// 要点: 静态库代码链进最终镜像; 动态库运行时加载、可共享与升级。
// 参考: ELF .a/.so; PE .lib/.dll

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

// 模拟「库函数」——本进程内链接，语义等同静态入库
[[maybe_unused]] int lib_add(int a, int b) {
    return a + b;
}
[[maybe_unused]] const char* lib_version() {
    return "1.0-demo";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G5 static vs dynamic library ===\n";

    assert(lib_add(2, 3) == 5);
    assert(std::string{lib_version()} == "1.0-demo");

    std::cout << "  static (.a / .lib archive):\n";
    std::cout << "    + self-contained deploy; + LTO / whole-program friendly\n";
    std::cout << "    - larger binaries; - library bugfix needs app relink\n";
    std::cout << "    - duplicate static libs if linked into multiple DSOs carefully\n";
    std::cout << "  dynamic (.so / .dll / .dylib):\n";
    std::cout << "    + share text pages; + patch library without app rebuild\n";
    std::cout << "    - ABI fragility across compilers/STL; - rpath/PATH hell\n";
    std::cout << "  Windows: import .lib is NOT the DLL — only thunks/metadata\n";
    std::cout << "  plugin systems almost always need dynamic loading (dlopen/LoadLibrary)\n";
    std::cout << "static_library_versus_dynamic_library: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/static_library_versus_dynamic_library", run>;

}  // namespace
