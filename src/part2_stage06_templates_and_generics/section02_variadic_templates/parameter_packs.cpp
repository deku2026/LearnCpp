// Parameter packs hold zero or more types, function arguments, or values.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : parameter_packs
// Topic id : part2/stage06/section02/parameter_packs
//
// TypeList and ValueList expose pack information without runtime type erasure.

#include "learn/example_support.hpp"

#include <cstddef>
#include <type_traits>

namespace {

template <class... Types>
struct TypeList {
    static constexpr std::size_t size = sizeof...(Types);
};

template <auto... Values>
struct ValueList {
    static constexpr std::size_t size = sizeof...(Values);
    static constexpr auto sum = (0 + ... + Values);
};

static_assert(TypeList<>::size == 0);
static_assert(TypeList<int, double, char>::size == 3);
static_assert(ValueList<1, 2, 3>::sum == 6);
static_assert(!std::is_same_v<TypeList<int>, TypeList<double>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section02/parameter_packs"};
    LEARN_EXPECT_EQ(checks, (TypeList<int, long>::size), std::size_t{2});
    LEARN_EXPECT_EQ(checks, (ValueList<2, 4, 8>::size), std::size_t{3});
    LEARN_EXPECT_EQ(checks, (ValueList<2, 4, 8>::sum), 14);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/parameter_packs", run>;

}  // namespace
