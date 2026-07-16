// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section04_template_metaprogramming
// Item     : std_integer_sequence
// Topic id : part6/e/section04/std_integer_sequence

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace {

template <class Tuple, std::size_t... Indices>
[[nodiscard]] constexpr auto tuple_to_array_impl(const Tuple& tuple, std::index_sequence<Indices...>) {
    using Common = std::common_type_t<std::tuple_element_t<Indices, Tuple>...>;
    return std::array<Common, sizeof...(Indices)>{static_cast<Common>(std::get<Indices>(tuple))...};
}

template <class... Values>
[[nodiscard]] constexpr auto tuple_to_array(const std::tuple<Values...>& tuple) {
    return tuple_to_array_impl(tuple, std::index_sequence_for<Values...>{});
}

template <std::size_t... Indices>
[[nodiscard]] constexpr std::size_t index_sum(std::index_sequence<Indices...>) noexcept {
    return (Indices + ... + 0U);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section04/std_integer_sequence"};

    constexpr auto values = tuple_to_array(std::tuple{1, 2L, 3LL});
    static_assert(std::is_same_v<std::remove_cv_t<decltype(values)>, std::array<long long, 3>>);
    static_assert(values == std::array<long long, 3>{1, 2, 3});
    static_assert(index_sum(std::make_index_sequence<5>{}) == 10);
    LEARN_EXPECT_EQ(checks, values[2], 3);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section04/std_integer_sequence", run>;

}  // namespace
