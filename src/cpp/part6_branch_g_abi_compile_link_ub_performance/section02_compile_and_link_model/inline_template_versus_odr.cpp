// LearnCpp topic
// Doc      : 第6部分-支线G · G4.3 inline / 模板与 ODR
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : inline_template_versus_odr
// Topic id : part6/g/section02/inline_template_versus_odr
//
// 要点: inline 与函数模板允许多 TU 定义但须相同; 链接器合并。
// 参考: [basic.def.odr] [temp]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

inline int scale(int x) {
    return x * 2;
}

template <typename T>
T identity(T x) {
    return x;
}

// C++17 inline 变量
inline int g_inline_counter = 0;

struct HeaderStyle {
    // 类内成员函数隐式 inline
    int twice(int x) const { return x * 2; }
};

// 显式实例化: 收口到单 TU 生成代码 (语法演示)
template std::string identity<std::string>(std::string);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G4 inline / template vs ODR ===\n";

    assert(scale(21) == 42);
    assert(identity(3.5) == 3.5);
    assert(HeaderStyle{}.twice(5) == 10);

    ++g_inline_counter;
    assert(g_inline_counter >= 1);

    std::cout << "  put templates/inline in headers; keep tokens identical\n";
    std::cout << "  explicit instantiation reduces compile time / code bloat\n";
    std::cout << "inline_template_versus_odr: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/inline_template_versus_odr", run>;

}  // namespace
