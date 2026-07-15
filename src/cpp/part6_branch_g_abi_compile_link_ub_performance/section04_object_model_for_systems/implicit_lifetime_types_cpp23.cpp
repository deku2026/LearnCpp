// LearnCpp topic
// Doc      : 第6部分-支线G · 隐式生命周期类型
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : implicit_lifetime_types_cpp23
// Topic id : part6/g/section04/implicit_lifetime_types_cpp23
//
// 要点: 隐式生命周期类型可在合适操作下被隐式创建;
//       标量、隐式生命周期类的数组、特定聚合等。
// 参考: [basic.types.general]; is_implicit_lifetime

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <new>
#include <string>
#include <type_traits>

namespace {

struct Trivial {
    int a;
    double b;
};

struct NonIL {
    std::string s;
};

template <typename T>
constexpr bool looks_implicit_lifetime = std::is_scalar_v<T> || (std::is_trivial_v<T> && std::is_standard_layout_v<T>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G9 implicit-lifetime types ===\n";

    static_assert(looks_implicit_lifetime<int>);
    static_assert(looks_implicit_lifetime<Trivial>);
    static_assert(!looks_implicit_lifetime<NonIL>);

#if defined(__cpp_lib_is_implicit_lifetime)
    static_assert(std::is_implicit_lifetime_v<int>);
    std::cout << "  std::is_implicit_lifetime_v available\n";
#else
    std::cout << "  approximate classification (stdlib trait optional)\n";
#endif

    // 数组元素: 对存储块做字节操作后作为 int 数组使用需遵循生命周期规则
    alignas(int) unsigned char storage[sizeof(int) * 4]{};
    int* arr = new (storage) int[4];
    arr[0] = 1;
    arr[1] = 2;
    assert(arr[0] + arr[1] == 3);
    // 数组 delete 对 placement 复杂; 元素平凡可跳过显式析构
    (void)arr;

    std::cout << "  use trait to gate raw-memory algorithms in templates\n";
    std::cout << "implicit_lifetime_types_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/implicit_lifetime_types_cpp23", run>;

}  // namespace
