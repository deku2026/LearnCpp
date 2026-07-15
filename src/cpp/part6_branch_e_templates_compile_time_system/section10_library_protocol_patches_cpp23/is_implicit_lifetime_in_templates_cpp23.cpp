// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E10 C++23 protocol)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section10_library_protocol_patches_cpp23
// Item     : is_implicit_lifetime_in_templates_cpp23
// Topic id : part6/e/section10/is_implicit_lifetime_in_templates_cpp23
//
// 要点: is_implicit_lifetime 标识可被隐式创建/开始生命周期的类型;
//       模板库写 allocator/序列化时用来约束 T。
// 参考: P2674; cppreference is_implicit_lifetime

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <type_traits>

#if defined(__cpp_lib_is_implicit_lifetime) || (defined(_MSC_VER) && _MSC_VER >= 1934) || defined(__clang__)
#include <type_traits>
// some libstdc++ ship trait under feature macro
#endif

namespace {

// 可移植探测: 若标准 trait 缺失, 用近似规则教学
// 隐式生命周期类型 (直觉): 标量 / 隐式生命周期类的数组 /
//   聚合且无用户提供特殊成员、无虚、无引用成员 等 (简化)

template <typename T>
inline constexpr bool approx_implicit_lifetime_v =
    std::is_scalar_v<T> || std::is_array_v<T> || (std::is_trivial_v<T> && std::is_standard_layout_v<T>);

template <typename T>
concept ByteBufferElement = approx_implicit_lifetime_v<T>;

template <ByteBufferElement T>
void fill_bytes(T* p, std::size_t n, T v) {
    for (std::size_t i = 0; i < n; ++i) {
        p[i] = v;
    }
}

struct TrivialPod {
    int a;
    double b;
};

struct NonTrivial {
    std::string s;
};

// 模板约束: 仅隐式生命周期可走"原始存储"路径
template <typename T>
constexpr const char* storage_strategy() {
    if constexpr (approx_implicit_lifetime_v<T>) {
        return "raw-storage-ok";
    } else {
        return "need-construct";
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E10 is_implicit_lifetime in templates (C++23) ===\n";

    static_assert(approx_implicit_lifetime_v<int>);
    static_assert(approx_implicit_lifetime_v<TrivialPod>);
    static_assert(!approx_implicit_lifetime_v<NonTrivial>);

#if defined(__cpp_lib_is_implicit_lifetime)
    static_assert(std::is_implicit_lifetime_v<int>);
    std::cout << "  std::is_implicit_lifetime_v available\n";
#else
    std::cout << "  using approximate trait (stdlib may lag)\n";
#endif

    int buf[4]{};
    fill_bytes(buf, 4, 7);
    assert(buf[3] == 7);

    assert(std::string(storage_strategy<int>()) == "raw-storage-ok");
    assert(std::string(storage_strategy<NonTrivial>()) == "need-construct");

    std::cout << "  template APIs: constrain memcpy/start_lifetime_as paths\n";
    std::cout << "  pairs with branch B/G object lifetime topics\n";
    std::cout << "is_implicit_lifetime_in_templates_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section10/is_implicit_lifetime_in_templates_cpp23", run>;

}  // namespace
