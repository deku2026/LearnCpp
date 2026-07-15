// LearnCpp topic
// Doc      : 第6部分-支线G · G5.2 符号解析
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : symbol_resolution
// Topic id : part6/g/section02/symbol_resolution
//
// 要点: 链接器解析未定义引用 → 定义; 强弱符号、库搜索顺序有影响。
// 参考: ld man page; PE linker

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>

namespace {

// 本 TU 提供定义
int resolved_value() {
    return 100;
}

// 通过函数指针模拟"解析到具体定义"
using Fn = int (*)();
Fn pick() {
    return &resolved_value;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G5 symbol resolution ===\n";

    Fn f = pick();
    assert(f() == 100);

    std::cout << "  undefined ref must find exactly one strong definition\n";
    std::cout << "  static libs: pulled only if needed (order matters on ld)\n";
    std::cout << "  --start-group / whole-archive for circular static deps\n";
    std::cout << "  weak symbols: overridable defaults (ELF)\n";
    std::cout << "symbol_resolution: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/symbol_resolution", run>;

}  // namespace
