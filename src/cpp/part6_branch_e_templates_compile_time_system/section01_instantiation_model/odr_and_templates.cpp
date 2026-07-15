// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E1 实例化模型)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section01_instantiation_model
// Item     : odr_and_templates
// Topic id : part6/e/section01/odr_and_templates
//
// 要点: 模板特化/成员函数可在多 TU 中定义, 但 ODR 要求"相同 token 序列";
//       inline / 头文件定义是常态; 显式实例化可收口符号。
// 参考: [basic.def.odr] [temp.spec] cppreference ODR / explicit instantiation

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

// 允许在多个 TU 出现相同定义的函数模板 (隐式 inline 语义由实例化规则保证)
template <typename T>
T identity(T x) {
    return x;
}

// 类模板的成员函数默认 inline (若在类内定义)
template <typename T>
struct Box {
    T value;
    T get() const { return value; }
    static constexpr const char* tag() { return "Box"; }
};

// 显式特化: 仍须在程序中对该特化最多一处非 inline 定义
// 这里用 inline 模拟头文件友好写法
template <>
inline int identity<int>(int x) {
    return x + 0;  // 与主模板语义等价, 演示特化入口
}

// 显式实例化声明: 告诉编译器"别处有定义", 本 TU 可不生成
// extern template class Box<double>;  // 多文件工程里使用

// ODR-use: 取地址 / 绑定引用会强制生成实体
template <typename T>
void touch_static() {
    // 调用静态成员 → ODR-use 该成员
    (void)Box<T>::tag();
}

// 函数模板局部静态: 每个特化一份, 跨 TU 合并为一
template <typename T>
int& per_type_counter() {
    static int n = 0;
    return n;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E1 ODR and templates ===\n";

    // --- 入门: 同一特化多次调用共享语义 ---
    assert(identity(3.5) == 3.5);
    assert(identity(10) == 10);
    Box<int> b{99};
    assert(b.get() == 99);
    std::cout << "  identity / Box work\n";

    // --- 进阶: ODR-use 触发实例化 ---
    touch_static<int>();
    touch_static<std::string>();
    assert(std::string(Box<int>::tag()) == "Box");
    std::cout << "  ODR-use of static members OK\n";

    // --- 专家: 每特化一份 local static ---
    per_type_counter<int>() = 1;
    per_type_counter<double>() = 2;
    ++per_type_counter<int>();
    assert(per_type_counter<int>() == 2);
    assert(per_type_counter<double>() == 2);
    std::cout << "  local static is per specialization\n";

    // 显式实例化定义 (本文件内演示语法; 真实工程放单独 .cpp)
    // template class Box<float>;
    // template int identity<int>(int);  // 若主模板未特化

    static_assert(std::is_same_v<decltype(identity(1)), int>);
    std::cout << "  tip: put templates in headers; use extern template to"
                 " cut compile time when needed.\n";
    std::cout << "odr_and_templates: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section01/odr_and_templates", run>;

}  // namespace
