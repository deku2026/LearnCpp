// Runnable teaching example
// Doc      : 第2部分-阶段14-特性与关键字全表.md
// Stage    : part2_stage14_keywords_and_features_index
// Section  : section02_features_by_standard_cpp14
// Item     : cpp14_library_features_overview
// Topic id : part2/stage14/section02/cpp14_library_features_overview
// References: C++23 [cpp.predefined], [cpp.cond], [support.limits.general], Annex C, Annex D; SD-FeatureTest

#include "learn/example_support.hpp"

#include <array>
#include <chrono>
#include <memory>
#include <string_view>
#include <tuple>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage14/section02/cpp14_library_features_overview";

template <std::size_t... Indices>
constexpr std::size_t index_sum(std::index_sequence<Indices...>) {
    std::size_t result{};
    const int expand[]{0, ((result += Indices), 0)...};
    (void)expand;
    return result;
}

constexpr std::array<std::string_view, 7> features{
    "make_unique",        "integer_sequence", "exchange",          "quoted",
    "shared_timed_mutex", "chrono literals",  "tuple get by type",
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto value = std::make_unique<int>(7);
    int state = 3;
    const int old = std::exchange(state, 9);
    const std::tuple<int, double> pair{4, 2.5};
    using namespace std::chrono_literals;
    static_assert(index_sum(std::make_index_sequence<4>{}) == 6U);
    LEARN_EXPECT_EQ(checks, *value, 7);
    LEARN_EXPECT_EQ(checks, old, 3);
    LEARN_EXPECT_EQ(checks, state, 9);
    LEARN_EXPECT_EQ(checks, std::get<double>(pair), 2.5);
    LEARN_EXPECT(checks, 1s == 1'000ms);
    LEARN_EXPECT_EQ(checks, features.size(), 7U);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage14/section02/cpp14_library_features_overview", run>;

}  // namespace
