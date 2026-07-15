// LearnCpp topic
// Doc      : 第6部分-支线G · G2 符号可见性
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : symbol_visibility
// Topic id : part6/g/section01/symbol_visibility
//
// 要点: 默认隐藏 + 显式导出公共 API; 减小动态符号表、稳定 ABI。
// 参考: GCC Visibility; Ulrich Drepper "How to Write Shared Libraries"

#include "learn/topic_registry.hpp"

#include <iostream>

// 跨平台导出宏 (平台特定扩展, 非 ISO)
#if defined(_WIN32)
#define LEARN_EXPORT __declspec(dllexport)
#define LEARN_HIDDEN
#elif defined(__GNUC__) || defined(__clang__)
#define LEARN_EXPORT __attribute__((visibility("default")))
#define LEARN_HIDDEN __attribute__((visibility("hidden")))
#else
#define LEARN_EXPORT
#define LEARN_HIDDEN
#endif

// dllexport / visibility(default) 需要外部链接 → 放命名命名空间
namespace learn_demo_vis {
LEARN_EXPORT int public_api(int x) {
    return x + 1;
}

LEARN_HIDDEN int internal_helper(int x) {
    return x * 2;
}
}  // namespace learn_demo_vis

namespace {

// 匿名命名空间: 内部链接, 不导出（不可与 dllexport 组合）
[[maybe_unused]] int tu_local(int x) {
    return x - 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G2 symbol visibility ===\n";

    assert(learn_demo_vis::public_api(1) == 2);
    assert(learn_demo_vis::internal_helper(3) == 6);
    assert(tu_local(5) == 4);

    std::cout << "  ELF: -fvisibility=hidden + default on API\n";
    std::cout << "  Windows: dllexport / dllimport\n";
    std::cout << "  benefits: smaller dynsym, fewer collisions, clearer ABI\n";
    std::cout << "  note: export attrs need external linkage (named ns)\n";
    std::cout << "symbol_visibility: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/symbol_visibility", run>;

}  // namespace
