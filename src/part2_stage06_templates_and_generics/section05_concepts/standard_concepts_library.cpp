// The standard concepts library supplies composable vocabulary constraints.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : standard_concepts_library
// Topic id : part2/stage06/section05/standard_concepts_library
//
// invocable and convertible_to describe a callable pipeline without spelling traits manually.

#include "learn/example_support.hpp"

#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>

namespace {

template <class Function, class T>
    requires std::invocable<Function&, T> && std::convertible_to<std::invoke_result_t<Function&, T>, T>
T apply_twice(Function function, T value) {
    T once = static_cast<T>(std::invoke(function, value));
    return static_cast<T>(std::invoke(function, once));
}

static_assert(std::integral<int>);
static_assert(std::floating_point<double>);
static_assert(std::same_as<std::remove_cvref_t<const int&>, int>);
static_assert(std::convertible_to<short, int>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section05/standard_concepts_library"};
    const auto add_three = [](int value) { return value + 3; };
    static_assert(std::invocable<decltype(add_three), int>);
    LEARN_EXPECT_EQ(checks, apply_twice(add_three, 4), 10);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/standard_concepts_library", run>;

}  // namespace
