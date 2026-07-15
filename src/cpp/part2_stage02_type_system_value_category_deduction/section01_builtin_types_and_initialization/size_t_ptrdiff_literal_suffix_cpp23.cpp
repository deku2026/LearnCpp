// LearnCpp topic example
// Doc      : part2-stage02-type-system-value-category-deduction.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : size_t_ptrdiff_literal_suffix_cpp23
// Topic id : part2/stage02/section01/size_t_ptrdiff_literal_suffix_cpp23
//
// Covers: size_t/ptrdiff_t, C++23 uz/z suffixes, signed/unsigned compare safety

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <type_traits>
#include <vector>
#include <version>

namespace {

void demo_basics() {
    std::size_t n = 10;
    std::ptrdiff_t d = -3;
    LEARN_CHECK(n == 10);
    LEARN_CHECK(d == -3);
    static_assert(std::is_unsigned_v<std::size_t>);
    static_assert(std::is_signed_v<std::ptrdiff_t>);
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3, 4, 5};
    const std::size_t sz = v.size();
    LEARN_CHECK(sz == 5);

    // Prefer size_t loop index to match container size type.
    std::size_t sum = 0;
    for (std::size_t i = 0; i < v.size(); ++i) {
        sum += static_cast<std::size_t>(v[i]);
    }
    LEARN_CHECK(sum == 15);

    const int arr[4] = {10, 20, 30, 40};
    const std::ptrdiff_t span = &arr[3] - &arr[0];
    LEARN_CHECK(span == 3);
}

void demo_expert() {
#if defined(__cpp_size_t_suffix) && __cpp_size_t_suffix >= 202011L
    auto uz = 10uz;
    auto z = 10z;
    static_assert(std::is_same_v<decltype(uz), std::size_t>);
    static_assert(std::is_same_v<decltype(z), std::make_signed_t<std::size_t>>);
    LEARN_CHECK(uz == 10);
    LEARN_CHECK(z == 10);

    std::vector<int> v{1, 2, 3};
    for (auto i = 0uz; i < v.size(); ++i) {
        LEARN_CHECK(v[i] == static_cast<int>(i) + 1);
    }
#else
    // Fallback: explicit casts avoid signed/unsigned mismatch.
    std::vector<int> v{1, 2, 3};
    for (std::size_t i = 0; i < v.size(); ++i) {
        LEARN_CHECK(v[i] == static_cast<int>(i) + 1);
    }
    static_assert(std::is_same_v<decltype(sizeof(0)), std::size_t>);
#endif

    static_assert(sizeof(std::size_t) == sizeof(void*) || sizeof(std::size_t) != sizeof(void*));
    LEARN_CHECK(sizeof(std::size_t) >= 4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/size_t_ptrdiff_literal_suffix_cpp23", run>;

}  // namespace
