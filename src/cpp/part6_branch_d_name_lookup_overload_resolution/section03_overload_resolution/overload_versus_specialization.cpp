// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D7 重载 vs 特化)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : overload_versus_specialization
// Topic id : part6/d/section03/overload_versus_specialization
//
// 要点: 函数模板显式特化不参与重载；用重载/CPO 做定制。
// 参考: Herb Sutter Why Not Specialize Function Templates

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

// 经典陷阱演示：特化附着到 f(T)，但重载选中 f(T*)
template <typename T>
std::string f(T) {
    return "f(T)";
}

template <>
std::string f(int*) {
    return "f<int*> specialization of f(T)";
}

template <typename T>
std::string f(T*) {
    return "f(T*) overload";
}

// 正确：重载（非模板）定制
template <typename T>
[[maybe_unused]] std::string g(T) {
    return "g(T)";
}

[[maybe_unused]] std::string g(int*) {
    return "g(int*) non-template overload";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D7 overload versus specialization ===\n";

    int x = 0;
    // 重载决议在基模板 f(T) 与 f(T*) 间选 → f(T*)；特化被绕过
    assert(f(&x) == "f(T*) overload");
    assert(f(x) == "f(T)");
    // 显式指定模板实参才会走到全特化（不要用特化做定制点）
    assert(f<int*>(&x) == "f<int*> specialization of f(T)");

    // 非模板重载参与且优先
    assert(g(&x) == "g(int*) non-template overload");
    assert(g(x) == "g(T)");

    std::cout << "  do not specialize function templates for customization\n";
    std::cout << "overload_versus_specialization: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/overload_versus_specialization", run>;

}  // namespace
