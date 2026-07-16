// C++17 auto non-type template parameters deduce both a value and its type.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : auto_nttp_cpp17
// Topic id : part2/stage06/section03/auto_nttp_cpp17
//
// The same template accepts int, char, unsigned long, and a function pointer.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

template <auto Value>
struct Constant {
    static constexpr auto value = Value;
    using value_type = decltype(Value);
};

constexpr int increment(int value) {
    return value + 1;
}

template <auto Function>
constexpr auto apply(int value) {
    return Function(value);
}

static_assert(std::is_same_v<Constant<42>::value_type, int>);
static_assert(std::is_same_v<Constant<'x'>::value_type, char>);
static_assert(std::is_same_v<Constant<7UL>::value_type, unsigned long>);
static_assert(apply<&increment>(4) == 5);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section03/auto_nttp_cpp17"};
    LEARN_EXPECT_EQ(checks, Constant<42>::value, 42);
    LEARN_EXPECT_EQ(checks, Constant<'x'>::value, 'x');
    LEARN_EXPECT_EQ(checks, apply<&increment>(9), 10);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section03/auto_nttp_cpp17", run>;

}  // namespace
