// LearnCpp topic
// Doc      : 第6部分-支线G · G4.1 翻译单元
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : translation_unit_deep
// Topic id : part6/g/section02/translation_unit_deep
//
// 要点: TU = 一个 .cpp + 其 #include 展开; 分别编译, 链接合并。
// 参考: [lex.phases] [basic.link]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>

namespace {

// 内部链接: 仅本 TU 可见
static int tu_counter = 0;
namespace {
int also_internal = 0;
}  // namespace

// 外部链接
int g_learn_tu_demo_unique = 0;  // 注意全局唯一名, 避免与其他 TU 冲突

[[maybe_unused]] inline int inline_func() {
    return 1;
}  // 可在多 TU 定义

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G4 translation unit deep ===\n";

    ++tu_counter;
    ++also_internal;
    ++g_learn_tu_demo_unique;
    assert(tu_counter == 1);
    assert(also_internal == 1);
    assert(inline_func() == 1);

    std::cout << "  compile: each TU → object file independently\n";
    std::cout << "  includes are textual; macros/leak across headers carefully\n";
    std::cout << "  modules (C++20) change the model: named ownership of decls\n";
    std::cout << "translation_unit_deep: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/translation_unit_deep", run>;

}  // namespace
