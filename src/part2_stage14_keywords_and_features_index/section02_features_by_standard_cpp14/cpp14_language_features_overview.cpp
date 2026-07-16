// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section02_features_by_standard_cpp14
// Item     : cpp14_language_features_overview
// Topic id : part2/stage14/section02/cpp14_language_features_overview
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <string_view>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section02/cpp14_language_features_overview";

template <class T>
constexpr T square_v = T{4} * T{4};

constexpr int factorial(int value) {
    int result = 1;
    for (int current = 2; current <= value; ++current) result *= current;
    return result;
}

auto make_pair_value(int value) {
    return std::pair{value, value * 2};
}

constexpr std::array<std::string_view, 7> features{
    "generic lambdas",      "lambda init-capture", "return type deduction",
    "relaxed constexpr",    "variable templates",  "binary literals/digit separators",
    "deprecated attribute",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto add = [](auto left, auto right) { return left + right; };
    auto captured = [value = 20 + 2] { return value; };
    static_assert(factorial(5) == 120);
    static_assert(square_v<int> == 16);
    LEARN_EXPECT_EQ(checks, add(2, 3), 5);
    LEARN_EXPECT_EQ(checks, captured(), 22);
    LEARN_EXPECT_EQ(checks, make_pair_value(4).second, 8);
    LEARN_EXPECT_EQ(checks, 0b1010'0101, 165);
    LEARN_EXPECT_EQ(checks, features.size(), 7U);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage14/section02/cpp14_language_features_overview", run>;

}  // namespace
