// LearnCpp topic
// Doc      : 第6部分-支线G · 符号剥离
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : symbol_stripping
// Topic id : part6/g/section02/symbol_stripping
//
// 要点: strip / -s 去掉调试符号; --gc-sections 去掉未引用节;
//       可见性隐藏帮助进一步裁剪。
// 参考: binutils strip; MSVC /OPT:REF

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>

namespace {

// 存活: 被调用
int used_func(int x) {
    return x + 1;
}

// 若链接 --gc-sections 且无引用, 目标节可丢弃
[[maybe_unused]] int unused_func(int x) {
    return x - 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G2/G5 symbol stripping ===\n";

    assert(used_func(41) == 42);

    std::cout << "  debug build: keep symbols for stack traces / profilers\n";
    std::cout << "  release: strip + section GC + LTO can shrink a lot\n";
    std::cout << "  never strip public API of a shared library you export\n";
    std::cout << "  split DWARF / PDB keeps debug info external\n";
    std::cout << "symbol_stripping: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/symbol_stripping", run>;

}  // namespace
