// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E10 C++23 protocol)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section10_library_protocol_patches_cpp23
// Item     : reference_constructs_from_temporary_in_templates_cpp23
// Topic id : part6/e/section10/reference_constructs_from_temporary_in_templates_cpp23
//
// 要点: reference_constructs_from_temporary / reference_converts_from_temporary
//       检测"绑定引用会悬空临时量" —— 模板可 static_assert 拦住。
// 参考: P2255; cppreference reference_constructs_from_temporary

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// 危险接口: 返回参数引用 —— 若传入临时则悬空
template <typename T>
const T& identity_ref(const T& x) {
    return x;
}

// 安全包装: 若会从临时构造引用则拒绝
template <typename T>
constexpr bool binds_temporary_v =
#if defined(__cpp_lib_reference_from_temporary)
    std::reference_constructs_from_temporary_v<const T&, T> ||
    std::reference_constructs_from_temporary_v<const T&, T&&>;
#else
    // 近似: 纯右值 prvalue 绑定到 const T& 会延长生命周期到完整表达式,
    // 但存下来的引用仍危险 —— 模板侧用 is_lvalue_reference 约束实参
    false;
#endif

template <typename T>
const T& safe_identity(const T& x) {
    // 要求调用方传入左值 (通过删除右值重载)
    return x;
}

template <typename T>
const T& safe_identity(const T&& x) = delete;

// 工厂返回临时
[[maybe_unused]] std::string make_name() {
    return "temp";
}

// 用 trait 做编译期文档断言 (有标准库时)
void trait_demo() {
#if defined(__cpp_lib_reference_from_temporary)
    // const std::string& 从 prvalue std::string 构造? → true (会绑定临时)
    constexpr bool danger = std::reference_constructs_from_temporary_v<const std::string&, std::string>;
    static_assert(danger);
    std::cout << "  trait says const string& constructs from temporary: " << danger << '\n';

    constexpr bool ok = std::reference_constructs_from_temporary_v<const std::string&, std::string&>;
    static_assert(!ok);
#else
    std::cout << "  std::reference_constructs_from_temporary not available;"
                 " using deleted rvalue overload pattern\n";
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E10 reference_constructs_from_temporary (C++23) ===\n";

    trait_demo();

    std::string live = "live";
    assert(safe_identity(live) == "live");
    assert(identity_ref(live) == "live");

    // 以下若取消注释则编译失败 (deleted rvalue overload):
    // auto& dangling = safe_identity(make_name());

    // 完整表达式内绑定临时是合法的, 但不要把引用存出表达式
    assert(identity_ref(std::string{"x"}) == "x");  // OK: 临时活过 assert 实参求值

    std::cout << "  template libraries should reject reference-to-temporary APIs\n";
    std::cout << "  C++23 traits enable precise static_assert diagnostics\n";
    std::cout << "reference_constructs_from_temporary_in_templates_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/e/section10/reference_constructs_from_temporary_in_templates_cpp23", run>;

}  // namespace
