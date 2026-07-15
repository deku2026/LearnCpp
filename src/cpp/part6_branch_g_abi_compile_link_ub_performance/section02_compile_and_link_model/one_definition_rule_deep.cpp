// LearnCpp topic
// Doc      : 第6部分-支线G · G4.2 ODR
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : one_definition_rule_deep
// Topic id : part6/g/section02/one_definition_rule_deep
//
// 要点: ODR — 实体至多一个定义; 跨 TU 相同实体定义须相同 token;
//       违反常静默 UB。
// 参考: [basic.def.odr]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

// 允许: 类定义在每个含其的 TU 中出现, 且相同
struct Point {
    int x;
    int y;
    int sum() const { return x + y; }
};

// 允许: inline 函数多 TU 相同定义
[[maybe_unused]] inline std::string greet(const std::string& n) {
    return "hi " + n;
}

// 允许: 模板定义在头文件
template <typename T>
T twice(T v) {
    return v + v;
}

// 常量: 内部链接 vs inline 变量
[[maybe_unused]] constexpr int kMagic = 42;
[[maybe_unused]] inline constexpr int kShared = 7;  // C++17 inline variable

// 违规示意 (不要在另一 .cpp 写不同的 Point 定义)

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G4 ODR deep ===\n";

    [[maybe_unused]] Point p{3, 4};
    assert(p.sum() == 7);
    assert(greet("Ada") == "hi Ada");
    assert(twice(21) == 42);
    assert(kMagic == 42);
    assert(kShared == 7);

    std::cout << "  linker catches duplicate non-inline functions\n";
    std::cout << "  silent UB: same class name, different members across TUs\n";
    std::cout << "  keep ODR-used inline/template defs identical\n";
    std::cout << "one_definition_rule_deep: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section02/one_definition_rule_deep", run>;

}  // namespace
