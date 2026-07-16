// A concept names a compile-time predicate that becomes part of a template interface.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : concept_definition
// Topic id : part2/stage06/section05/concept_definition
//
// Arithmetic is trait-based; Addable describes a required expression and result type.

#include "learn/example_support.hpp"

#include <concepts>
#include <string>
#include <type_traits>

namespace {

template <class T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

template <class T>
concept Addable = requires(const T& left, const T& right) {
    { left + right } -> std::same_as<T>;
};

template <Arithmetic T>
constexpr T midpoint(T left, T right) {
    return left + (right - left) / static_cast<T>(2);
}

static_assert(Arithmetic<int>);
static_assert(Arithmetic<double>);
static_assert(!Arithmetic<std::string>);
static_assert(Addable<std::string>);
static_assert(midpoint(2, 8) == 5);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section05/concept_definition"};
    LEARN_EXPECT_EQ(checks, midpoint(10, 20), 15);
    LEARN_EXPECT_EQ(checks, midpoint(1.0, 2.0), 1.5);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/concept_definition", run>;

}  // namespace
