// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section01_features_by_standard_cpp11
// Item     : cpp11_language_features_overview
// Topic id : part2/stage14/section01/cpp11_language_features_overview
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <concepts>
#include <memory>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section01/cpp11_language_features_overview";

enum class Color { red, blue };

template <class T>
constexpr T sum(T value) {
    return value;
}

template <class T, class... Rest>
constexpr auto sum(T first, Rest... rest) -> typename std::common_type<T, Rest...>::type {
    return first + sum(rest...);
}

constexpr std::array<std::string_view, 12> features{
    "auto/decltype", "range-for",          "lambdas", "rvalue references",       "move semantics", "constexpr",
    "scoped enums",  "variadic templates", "nullptr", "delegating constructors", "override/final", "static_assert",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto owner = std::make_unique<int>(7);
    const auto twice = [](int value) { return value * 2; };
    std::vector<int> values{1, 2, 3};
    int total{};
    for (const int value : values) total += value;
    static_assert(std::same_as<decltype(nullptr), std::nullptr_t>);
    static_assert(sum(1, 2, 3) == 6);
    LEARN_EXPECT_EQ(checks, twice(*owner), 14);
    LEARN_EXPECT_EQ(checks, total, 6);
    LEARN_EXPECT_EQ(checks, features.size(), 12U);
    LEARN_EXPECT_EQ(checks, Color::blue, Color::blue);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage14/section01/cpp11_language_features_overview", run>;

}  // namespace
