// A function template is instantiated into type-specific functions on demand.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : function_template
// Topic id : part2/stage06/section01/function_template
//
// Its operations form an implicit interface: this maximum requires ordering.

#include "learn/example_support.hpp"

#include <concepts>
#include <string>
#include <type_traits>

namespace {

template <class T>
constexpr T maximum(T left, T right) {
    return left < right ? right : left;
}

template <class T>
concept HasMaximum = requires(T value) {
    { maximum(value, value) } -> std::same_as<T>;
};

static_assert(maximum(3, 5) == 5);
static_assert(std::is_same_v<decltype(maximum(1.0, 2.0)), double>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section01/function_template"};
    static_assert(HasMaximum<int>);
    LEARN_EXPECT_EQ(checks, maximum(8, 3), 8);
    LEARN_EXPECT_EQ(checks, maximum(std::string{"alpha"}, std::string{"beta"}), std::string{"beta"});
    LEARN_EXPECT_EQ(checks, maximum<double>(2, 3.5), 3.5);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/function_template", run>;

}  // namespace
