// LearnCpp topic
// Doc      : 第6部分-支线G · G5 静态 vs 动态库
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

// 模拟"库函数"
int lib_add(int a, int b) {
    return a + b;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G5 static vs dynamic library ===\n";

    assert(lib_add(2, 3) == 5);

    std::cout << "  static (.a/.lib):\n";
    std::cout << "    + self-contained deploy; + LTO friendly\n";
    std::cout << "    - larger binaries; - update needs relink\n";
    std::cout << "  dynamic (.so/.dll/.dylib):\n";
    std::cout << "    + share pages; + patch library without app rebuild\n";
    std::cout << "    - ABI fragility; - load path / version hell\n";
    std::cout << "  import lib on Windows is not the DLL — just thunks\n";
    std::cout << "static_library_versus_dynamic_library: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/static_library_versus_dynamic_library", run>;

}  // namespace
