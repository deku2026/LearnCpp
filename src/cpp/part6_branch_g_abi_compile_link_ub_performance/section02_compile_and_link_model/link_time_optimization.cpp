// LearnCpp topic
// Doc      : 第6部分-支线G · G6 LTO
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : link_time_optimization
// Topic id : part6/g/section02/link_time_optimization
//
// 要点: LTO 在链接期跨 TU 内联/消虚拟/常量传播; 增加链接时间。
// 参考: GCC -flto; Clang -flto; MSVC /GL + /LTCG

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>

namespace {

// 小函数: LTO 可跨 TU 内联 (此处同 TU 已可内联)
inline int hot_add(int a, int b) {
    return a + b;
}

int cold_path(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

int pipeline(int x) {
    return hot_add(cold_path(x), 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G6 link-time optimization ===\n";

    assert(pipeline(3) == 4);
    assert(pipeline(-5) == 6);

    std::cout << "  enable: -flto / -flto=thin /GL+/LTCG\n";
    std::cout << "  wins: cross-TU inlining, devirtualization, DCE\n";
    std::cout << "  costs: link RAM/time; harder to bisect object code\n";
    std::cout << "  fat LTO vs ThinLTO tradeoffs\n";
    std::cout << "link_time_optimization: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/link_time_optimization", run>;

}  // namespace
