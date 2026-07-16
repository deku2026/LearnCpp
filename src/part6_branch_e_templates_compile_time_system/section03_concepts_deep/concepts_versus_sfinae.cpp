// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section03_concepts_deep
// Item     : concepts_versus_sfinae
// Topic id : part6/e/section03/concepts_versus_sfinae

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <concepts>
#include <string_view>
#include <type_traits>

namespace {

template <class T>
using legacy_incrementable_t = std::void_t<decltype(++std::declval<T&>()), decltype(std::declval<T&>()++)>;

template <class T, class = legacy_incrementable_t<T>>
[[nodiscard]] constexpr T legacy_twice(T value) {
    ++value;
    ++value;
    return value;
}

template <class T>
concept Incrementable = requires(T value) {
    { ++value } -> std::same_as<T&>;
    value++;
};

template <Incrementable T>
[[nodiscard]] constexpr T constrained_twice(T value) {
    ++value;
    ++value;
    return value;
}

template <class T>
concept LegacyAccepts = requires(T value) { legacy_twice(value); };

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section03/concepts_versus_sfinae"};

    static_assert(Incrementable<int>);
    static_assert(!Incrementable<std::string_view>);
    static_assert(LegacyAccepts<int>);
    static_assert(!LegacyAccepts<std::string_view>);
    static_assert(legacy_twice(40) == constrained_twice(40));
    LEARN_EXPECT_EQ(checks, constrained_twice(40), 42);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section03/concepts_versus_sfinae", run>;

}  // namespace
