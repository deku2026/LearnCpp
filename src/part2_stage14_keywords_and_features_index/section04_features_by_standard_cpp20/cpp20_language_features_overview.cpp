// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section04_features_by_standard_cpp20
// Item     : cpp20_language_features_overview
// Topic id : part2/stage14/section04/cpp20_language_features_overview
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <compare>
#include <concepts>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section04/cpp20_language_features_overview";

template <std::integral T>
constexpr T twice(T value) {
    return value + value;
}

consteval int compile_time_id(int value) {
    return value * 10;
}
constinit int initialized_before_dynamic = 7;

struct Point {
    int x;
    int y;
    auto operator<=>(const Point&) const = default;
};

constexpr std::array<std::string_view, 12> features{
    "concepts",         "requires",        "spaceship", "consteval",
    "constinit",        "coroutines",      "modules",   "designated initializers",
    "lambda templates", "likely/unlikely", "char8_t",   "aggregate paren init",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr Point first{1, 2};
    constexpr Point second{2, 0};
    static_assert(first < second);
    static_assert(twice(4) == 8);
    constexpr int id = compile_time_id(3);
    LEARN_EXPECT_EQ(checks, id, 30);
    LEARN_EXPECT_EQ(checks, initialized_before_dynamic, 7);
    LEARN_EXPECT_EQ(checks, features.size(), 12U);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage14/section04/cpp20_language_features_overview", run>;

}  // namespace
