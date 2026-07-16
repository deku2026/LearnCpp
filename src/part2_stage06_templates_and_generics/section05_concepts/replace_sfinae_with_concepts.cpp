// A named concept expresses the same restriction as enable_if with less machinery.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section05_concepts
// Item     : replace_sfinae_with_concepts
// Topic id : part2/stage06/section05/replace_sfinae_with_concepts
//
// Unsupported calls are queried in dependent requires-expressions, never activated.

#include "learn/example_support.hpp"

#include <concepts>
#include <type_traits>

namespace {

template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
constexpr T legacy_increment(T value) {
    return value + 1;
}

template <std::integral T>
constexpr T modern_increment(T value) {
    return value + 1;
}

template <class T>
concept CanLegacyIncrement = requires(T value) { legacy_increment(value); };

template <class T>
concept CanModernIncrement = requires(T value) { modern_increment(value); };

static_assert(CanLegacyIncrement<int> && CanModernIncrement<int>);
static_assert(!CanLegacyIncrement<double> && !CanModernIncrement<double>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section05/replace_sfinae_with_concepts"};
    LEARN_EXPECT_EQ(checks, legacy_increment(4), 5);
    LEARN_EXPECT_EQ(checks, modern_increment(4), 5);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section05/replace_sfinae_with_concepts", run>;

}  // namespace
