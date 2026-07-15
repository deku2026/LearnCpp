// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B8 is_implicit_lifetime)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section05_lifetime_traits_cpp23
// Item     : is_implicit_lifetime_cpp23
// Topic id : part6/b/section05/is_implicit_lifetime_cpp23
//
// 要点: 隐式生命周期类型可用 start_lifetime_as / 某些底层字节解释；
//       非平凡类型需 placement new。
// 参考: https://en.cppreference.com/w/cpp/types/is_implicit_lifetime

#include "learn/topic_registry.hpp"

#include <cassert>
#include <complex>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

struct Aggregate {
    int x;
    double y;
};

struct NonTrivial {
    std::string s;
    NonTrivial() : s("x") {}
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B8 is_implicit_lifetime (C++23) ===\n";

#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    static_assert(std::is_implicit_lifetime_v<int>);
    static_assert(std::is_implicit_lifetime_v<double>);
    static_assert(std::is_implicit_lifetime_v<Aggregate>);
    static_assert(std::is_implicit_lifetime_v<std::complex<float>>);
    static_assert(!std::is_implicit_lifetime_v<std::string>);
    static_assert(!std::is_implicit_lifetime_v<NonTrivial>);

    assert(std::is_implicit_lifetime_v<int>);
    assert(!std::is_implicit_lifetime_v<std::string>);

    std::cout << "  implicit-lifetime: scalars/aggregates/trivial-ish types\n";
    std::cout << "  non-trivial (string): need construct_at / placement new\n";
#else
    // 回退：用 trivially 相关 traits 近似说明
    static_assert(std::is_trivially_default_constructible_v<int>);
    static_assert(!std::is_trivially_default_constructible_v<std::string>);
    std::cout << "  is_implicit_lifetime not available; triviality note only\n";
#endif

    std::cout << "is_implicit_lifetime_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section05/is_implicit_lifetime_cpp23", run>;

}  // namespace
