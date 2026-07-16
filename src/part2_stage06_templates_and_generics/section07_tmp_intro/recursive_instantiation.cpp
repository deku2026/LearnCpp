// Recursive template instantiation computes values through smaller specializations.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section07_tmp_intro
// Item     : recursive_instantiation
// Topic id : part2/stage06/section07/recursive_instantiation
//
// Full specializations provide explicit termination cases before unsigned underflow.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

template <unsigned Value>
struct Factorial : std::integral_constant<unsigned long long, Value * Factorial<Value - 1>::value> {};

template <>
struct Factorial<0> : std::integral_constant<unsigned long long, 1> {};

template <unsigned Value>
struct Fibonacci
    : std::integral_constant<unsigned long long, Fibonacci<Value - 1>::value + Fibonacci<Value - 2>::value> {};

template <>
struct Fibonacci<0> : std::integral_constant<unsigned long long, 0> {};

template <>
struct Fibonacci<1> : std::integral_constant<unsigned long long, 1> {};

static_assert(Factorial<6>::value == 720);
static_assert(Fibonacci<10>::value == 55);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section07/recursive_instantiation"};
    LEARN_EXPECT_EQ(checks, Factorial<8>::value, 40'320ULL);
    LEARN_EXPECT_EQ(checks, Fibonacci<12>::value, 144ULL);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section07/recursive_instantiation", run>;

}  // namespace
