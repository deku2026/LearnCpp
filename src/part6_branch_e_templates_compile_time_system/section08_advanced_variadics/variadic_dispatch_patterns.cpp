// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section08_advanced_variadics
// Item     : variadic_dispatch_patterns
// Topic id : part6/e/section08/variadic_dispatch_patterns

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <functional>
#include <string>
#include <tuple>
#include <utility>
#include <variant>

namespace {

template <class... Callables>
struct overloaded : Callables... {
    using Callables::operator()...;
};

template <class... Callables>
overloaded(Callables...) -> overloaded<Callables...>;

template <class Function, class... Values>
constexpr void for_each_argument(Function&& function, Values&&... values) {
    (std::invoke(function, std::forward<Values>(values)), ...);
}

template <class Tuple, class Function, std::size_t... Indices>
[[nodiscard]] constexpr auto transform_tuple_impl(Tuple&& tuple, Function&& function, std::index_sequence<Indices...>) {
    return std::tuple{std::invoke(function, std::get<Indices>(std::forward<Tuple>(tuple)))...};
}

template <class Tuple, class Function>
[[nodiscard]] constexpr auto transform_tuple(Tuple&& tuple, Function&& function) {
    constexpr auto size = std::tuple_size_v<std::remove_reference_t<Tuple>>;
    return transform_tuple_impl(std::forward<Tuple>(tuple), std::forward<Function>(function),
                                std::make_index_sequence<size>{});
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section08/variadic_dispatch_patterns"};

    std::variant<int, std::string> value = std::string{"cpp"};
    const auto length = std::visit(overloaded{[](int number) { return number; },
                                              [](const std::string& text) { return static_cast<int>(text.size()); }},
                                   value);
    LEARN_EXPECT_EQ(checks, length, 3);

    int sum = 0;
    for_each_argument([&sum](int item) { sum += item; }, 10, 20, 12);
    LEARN_EXPECT_EQ(checks, sum, 42);

    constexpr auto doubled = transform_tuple(std::tuple{1, 2, 3}, [](int item) { return item * 2; });
    static_assert(doubled == std::tuple{2, 4, 6});
    LEARN_EXPECT_EQ(checks, std::get<2>(doubled), 6);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section08/variadic_dispatch_patterns", run>;

}  // namespace
