// Template argument deduction depends on the parameter pattern.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : template_argument_deduction
// Topic id : part2/stage06/section01/template_argument_deduction
//
// By-value deduction removes top-level const and decays arrays; references
// preserve const and can deduce an array extent.

#include "learn/example_support.hpp"

#include <cstddef>
#include <type_traits>

namespace {

template <class T>
constexpr bool value_parameter_is_const(T) {
    return std::is_const_v<T>;
}

template <class T>
constexpr bool reference_parameter_is_const(T&) {
    return std::is_const_v<T>;
}

template <class T, std::size_t Size>
constexpr std::size_t array_extent(T (&)[Size]) {
    return Size;
}

static_assert(!value_parameter_is_const(1));

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section01/template_argument_deduction"};
    const int constant = 7;
    int array[5]{};
    static_assert(array_extent(array) == 5);
    LEARN_EXPECT(checks, !value_parameter_is_const(constant));
    LEARN_EXPECT(checks, reference_parameter_is_const(constant));
    LEARN_EXPECT_EQ(checks, array_extent(array), std::size_t{5});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/template_argument_deduction", run>;

}  // namespace
