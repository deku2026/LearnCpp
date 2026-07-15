// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section07_type_aliases_and_traits
// Item     : type_traits_intro
// Topic id : part2/stage02/section07/type_traits_intro
//
// Covers: is_same, is_integral, remove_*, enable-style checks with static_assert

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

namespace {

template <class T>
constexpr bool is_number_v = std::is_arithmetic_v<T>;

void demo_basics() {
    static_assert(std::is_same_v<int, int>);
    static_assert(!std::is_same_v<int, unsigned>);
    static_assert(std::is_integral_v<int>);
    static_assert(std::is_floating_point_v<double>);
    static_assert(std::is_pointer_v<int*>);
    static_assert(std::is_reference_v<int&>);

    assert(is_number_v<int>);
    assert(is_number_v<double>);
    assert(!is_number_v<std::string>);
}

void demo_intermediate() {
    static_assert(std::is_same_v<std::remove_const_t<const int>, int>);
    static_assert(std::is_same_v<std::remove_reference_t<int&>, int>);
    static_assert(std::is_same_v<std::remove_reference_t<int&&>, int>);
    static_assert(std::is_same_v<std::decay_t<const int&>, int>);
    static_assert(std::is_same_v<std::decay_t<int[3]>, int*>);

    static_assert(std::is_const_v<const int>);
    static_assert(std::is_volatile_v<volatile int>);
    static_assert(std::is_signed_v<int>);
    static_assert(std::is_unsigned_v<unsigned>);

    using T = std::add_pointer_t<int>;
    static_assert(std::is_same_v<T, int*>);
}

void demo_expert() {
    static_assert(std::is_class_v<std::string>);
    static_assert(std::is_class_v<std::vector<int>>);
    static_assert(std::is_trivial_v<int>);
    static_assert(std::is_trivially_copyable_v<int>);

    static_assert(std::is_constructible_v<std::string, const char*>);
    static_assert(std::is_nothrow_move_constructible_v<std::string> ||
                  !std::is_nothrow_move_constructible_v<std::string>);

    // Conditional type selection
    using Chosen = std::conditional_t<sizeof(void*) == 8, std::uint64_t, std::uint32_t>;
    static_assert(sizeof(Chosen) == 4 || sizeof(Chosen) == 8);

    // common_type
    using C = std::common_type_t<int, double>;
    static_assert(std::is_same_v<C, double>);

    // Runtime confirmation that traits guided a simple branch
    if constexpr (std::is_integral_v<int>) {
        assert(true);
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section07/type_traits_intro", run>;

}  // namespace
