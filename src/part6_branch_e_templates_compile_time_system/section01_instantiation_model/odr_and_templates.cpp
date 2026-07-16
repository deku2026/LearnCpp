// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section01_instantiation_model
// Item     : odr_and_templates
// Topic id : part6/e/section01/odr_and_templates

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <concepts>
#include <cstddef>
#include <type_traits>

namespace {

template <class T>
inline constexpr std::size_t object_width = sizeof(T);

template <class T>
[[nodiscard]] constexpr T square(T value) {
    return value * value;
}

template <class T>
struct TypeCategory {
    static constexpr int value = 0;
};

template <std::integral T>
struct TypeCategory<T> {
    static constexpr int value = 1;
};

template <>
struct TypeCategory<bool> {
    static constexpr int value = 2;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section01/odr_and_templates"};

    static_assert(object_width<int> == sizeof(int));
    static_assert(square(6) == 36);
    static_assert(TypeCategory<double>::value == 0);
    static_assert(TypeCategory<int>::value == 1);
    static_assert(TypeCategory<bool>::value == 2);

    // Identical template definitions may appear in several translation units.
    // Each specialization still denotes one ODR entity in the linked program.
    constexpr auto* first_view = &object_width<long long>;
    constexpr auto* second_view = &object_width<long long>;
    LEARN_EXPECT_EQ(checks, first_view, second_view);
    LEARN_EXPECT_EQ(checks, square(7), 49);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section01/odr_and_templates", run>;

}  // namespace
