// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section03_features_by_standard_cpp17
// Item     : cpp17_language_features_overview
// Topic id : part2/stage14/section03/cpp17_language_features_overview
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section03/cpp17_language_features_overview";

template <class... Values>
constexpr auto product(Values... values) {
    return (values * ...);
}

template <class T>
std::string classify(const T&) {
    if constexpr (std::is_integral_v<T>)
        return "integral";
    else
        return "other";
}

constexpr std::array<std::string_view, 10> features{
    "structured bindings",     "if/switch init",    "if constexpr",      "fold expressions", "inline variables", "CTAD",
    "guaranteed copy elision", "nested namespaces", "constexpr lambdas", "attributes",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto [name, score] = std::tuple{std::string{"Ada"}, 10};
    if (const auto length = name.size(); length == 3) LEARN_EXPECT_EQ(checks, length, 3U);
    static_assert(product(2, 3, 4) == 24);
    LEARN_EXPECT_EQ(checks, score, 10);
    LEARN_EXPECT_EQ(checks, classify(3), std::string{"integral"});
    LEARN_EXPECT_EQ(checks, features.size(), 10U);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage14/section03/cpp17_language_features_overview", run>;

}  // namespace
