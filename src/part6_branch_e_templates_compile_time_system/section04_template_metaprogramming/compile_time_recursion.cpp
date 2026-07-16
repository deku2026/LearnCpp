// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section04_template_metaprogramming
// Item     : compile_time_recursion
// Topic id : part6/e/section04/compile_time_recursion

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <cstddef>
#include <type_traits>

namespace {

template <unsigned int N>
struct factorial : std::integral_constant<unsigned long long, N * factorial<N - 1>::value> {};

template <>
struct factorial<0> : std::integral_constant<unsigned long long, 1> {};

template <class... Values>
struct type_count;

template <>
struct type_count<> : std::integral_constant<std::size_t, 0> {};

template <class First, class... Rest>
struct type_count<First, Rest...> : std::integral_constant<std::size_t, 1 + type_count<Rest...>::value> {};

[[nodiscard]] constexpr unsigned long long iterative_factorial(unsigned int value) noexcept {
    unsigned long long result = 1;
    for (unsigned int factor = 2; factor <= value; ++factor) {
        result *= factor;
    }
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section04/compile_time_recursion"};

    static_assert(factorial<0>::value == 1);
    static_assert(factorial<10>::value == 3'628'800);
    static_assert(type_count<int, double, char>::value == 3);
    static_assert(iterative_factorial(10) == factorial<10>::value);
    LEARN_EXPECT_EQ(checks, iterative_factorial(6), 720U);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section04/compile_time_recursion", run>;

}  // namespace
