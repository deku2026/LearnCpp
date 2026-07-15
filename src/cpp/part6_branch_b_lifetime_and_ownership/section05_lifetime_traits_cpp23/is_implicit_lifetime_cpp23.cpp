// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B8 is_implicit_lifetime) + 支线 C/G
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section05_lifetime_traits_cpp23
// Item     : is_implicit_lifetime_cpp23
// Topic id : part6/b/section05/is_implicit_lifetime_cpp23
//
// 要点: 隐式生命周期类型可用 start_lifetime_as / 某些字节解释路径；
//       非平凡类型必须 construct_at / placement new。
// 参考: https://en.cppreference.com/w/cpp/types/is_implicit_lifetime  P2674

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <complex>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace {

struct Aggregate {
    int x;
    double y;
};

struct NonTrivial {
    std::string s;
    NonTrivial() : s("x") {}
};

struct Empty {};

template <class T>
void show(const char* name) {
#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    std::cout << "  " << name << ": implicit_lifetime=" << std::is_implicit_lifetime_v<T> << " trivial="
              << std::is_trivial_v<T> << '\n';
#else
    std::cout << "  " << name << ": trivial_default_ctor=" << std::is_trivially_default_constructible_v<T> << '\n';
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B8 is_implicit_lifetime (C++23) ===\n";

#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    // --- 入门 ---
    static_assert(std::is_implicit_lifetime_v<int>);
    static_assert(std::is_implicit_lifetime_v<double>);
    static_assert(std::is_implicit_lifetime_v<Aggregate>);
    static_assert(std::is_implicit_lifetime_v<std::complex<float>>);
    static_assert(std::is_implicit_lifetime_v<Empty>);
    static_assert(!std::is_implicit_lifetime_v<std::string>);
    static_assert(!std::is_implicit_lifetime_v<NonTrivial>);
    static_assert(!std::is_implicit_lifetime_v<std::vector<int>>);

    assert(std::is_implicit_lifetime_v<int>);
    assert(std::is_implicit_lifetime_v<Aggregate>);
    assert(!std::is_implicit_lifetime_v<std::string>);

    // --- 进阶: 数组 ---
    static_assert(std::is_implicit_lifetime_v<int[4]>);
    // std::array 是否 implicit-lifetime 依赖元素与标准细化；不在此强制 static_assert

    show<int>("int");
    show<Aggregate>("Aggregate");
    show<std::string>("string");
    show<NonTrivial>("NonTrivial");

    // --- 专家: 工程含义 ---
    // 隐式生命周期 ≈ 可在字节缓冲上「开始对象生命」而不跑用户构造
    // → 序列化缓冲、mmap、网络包解析的类型筛选
    // 非隐式（string/vector）→ 必须正式构造，否则生命周期/不变量未建立
    std::cout << "  implicit-lifetime → candidate for start_lifetime_as (branch C)\n";
    std::cout << "  non-trivial string-like → construct_at / placement new only\n";
#else
    static_assert(std::is_trivially_default_constructible_v<int>);
    static_assert(std::is_trivially_default_constructible_v<Aggregate>);
    static_assert(!std::is_trivially_default_constructible_v<std::string>);
    show<int>("int");
    show<std::string>("string");
    std::cout << "  is_implicit_lifetime not available; triviality note only\n";
#endif

    std::cout << "is_implicit_lifetime_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section05/is_implicit_lifetime_cpp23", run>;

}  // namespace
