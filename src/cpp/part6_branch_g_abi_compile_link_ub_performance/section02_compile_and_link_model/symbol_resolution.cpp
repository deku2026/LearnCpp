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

int resolved_value() {
    return 100;
}

using Fn = int (*)();
Fn pick() {
    return &resolved_value;
}

// ODR: 同一实体一个定义（本演示单 TU）
[[maybe_unused]] inline int inline_helper() {
    return 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G5 symbol resolution ===\n";

    [[maybe_unused]] Fn f = pick();
    assert(f() == 100);
    assert(inline_helper() == 1);

    std::cout << "  undefined ref must find exactly one strong definition\n";
    std::cout << "  static libs: archive members pulled only if needed (ld order)\n";
    std::cout << "  --start-group / whole-archive for circular static deps\n";
    std::cout << "  weak symbols: overridable defaults (ELF)\n";
    std::cout << "  duplicate strong symbols: link error (or ODR UB if silent)\n";
    std::cout << "symbol_resolution: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/symbol_resolution", run>;

}  // namespace
