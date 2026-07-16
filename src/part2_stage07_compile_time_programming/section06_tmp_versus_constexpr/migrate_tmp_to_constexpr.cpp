// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section06_tmp_versus_constexpr
// Item     : migrate_tmp_to_constexpr
// Topic id : part2/stage07/section06/migrate_tmp_to_constexpr
// References: N4950 [expr.const], [temp], [meta.type.synop].

#include "learn/example_support.hpp"

#include <cstdint>
#include <type_traits>

namespace {

// Classic value-level TMP: each step instantiates another class template.
template <unsigned Value>
struct TmpFactorial : std::integral_constant<std::uint64_t, Value * TmpFactorial<Value - 1>::value> {};

template <>
struct TmpFactorial<0> : std::integral_constant<std::uint64_t, 1> {};

// Modern value-level computation: ordinary control flow, usable at both
// compile time and runtime, and much easier to inspect or debug.
constexpr std::uint64_t constexpr_factorial(unsigned value) noexcept {
    std::uint64_t result = 1;
    for (unsigned factor = 2; factor <= value; ++factor) {
        result *= factor;
    }
    return result;
}

// Type-level transformations still belong to templates/type traits because a
// C++ type is not a value that a constexpr function can return.
template <class Type>
using ConstPointer = std::add_pointer_t<std::add_const_t<Type>>;

static_assert(TmpFactorial<0>::value == constexpr_factorial(0));
static_assert(TmpFactorial<5>::value == constexpr_factorial(5));
static_assert(TmpFactorial<10>::value == constexpr_factorial(10));
static_assert(std::is_same_v<ConstPointer<int>, const int*>);

int run(int argc, char** argv) {
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage07/section06/migrate_tmp_to_constexpr"};

    unsigned runtime_argument = 6U;
    runtime_argument += static_cast<unsigned>(argc - argc);
    LEARN_EXPECT_EQ(checks, constexpr_factorial(runtime_argument), 720ULL);
    LEARN_EXPECT_EQ(checks, TmpFactorial<6>::value, constexpr_factorial(6));
    LEARN_EXPECT_EQ(checks, constexpr_factorial(0), 1ULL);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section06/migrate_tmp_to_constexpr", run>;

}  // namespace
