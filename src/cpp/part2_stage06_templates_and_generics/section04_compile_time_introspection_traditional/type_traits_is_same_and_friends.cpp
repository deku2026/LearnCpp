// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : type_traits_is_same_and_friends
// Topic id : part2/stage06/section04/type_traits_is_same_and_friends
//
// Covers: is_same, is_integral, remove_*, conditional_t basics

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>

namespace {

template <typename T>
constexpr bool is_number_like_v = std::is_integral_v<T> || std::is_floating_point_v<T>;

template <typename T>
using DecayNoPtr = std::remove_pointer_t<std::decay_t<T>>;

void demo_basics() {
    static_assert(std::is_same_v<int, int>);
    static_assert(!std::is_same_v<int, unsigned>);
    static_assert(std::is_integral_v<int>);
    static_assert(std::is_floating_point_v<double>);
    assert(is_number_like_v<int>);
    assert(is_number_like_v<float>);
    assert(!is_number_like_v<std::string>);
}

void demo_intermediate() {
    static_assert(std::is_same_v<std::remove_const_t<const int>, int>);
    static_assert(std::is_same_v<std::remove_reference_t<int&>, int>);
    static_assert(std::is_same_v<std::decay_t<int[3]>, int*>);
    static_assert(std::is_same_v<DecayNoPtr<const int*&>, const int>);
    static_assert(std::is_same_v<DecayNoPtr<int*>, int>);
}

void demo_expert() {
    using A = std::conditional_t<true, int, double>;
    using B = std::conditional_t<false, int, double>;
    static_assert(std::is_same_v<A, int>);
    static_assert(std::is_same_v<B, double>);

    static_assert(std::is_const_v<const int>);
    static_assert(std::is_lvalue_reference_v<int&>);
    static_assert(std::is_rvalue_reference_v<int&&>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/type_traits_is_same_and_friends", run>;

}  // namespace
