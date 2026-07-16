// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_tuple_and_pair
// Topic id : part2/stage10/section06/std_tuple_and_pair

#include "learn/example_support.hpp"

#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/std_tuple_and_pair";

struct Point {
    int x;
    int y;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    std::pair<std::string, int> score{"Ada", 90};
    auto& [name, points] = score;
    points += 5;
    LEARN_EXPECT_EQ(checks, name, "Ada");
    LEARN_EXPECT_EQ(checks, score.second, 95);

    int id{};
    std::string status;
    double latency{};
    std::tie(id, status, latency) = std::tuple{17, "ready", 2.5};
    LEARN_EXPECT_EQ(checks, id, 17);
    LEARN_EXPECT_EQ(checks, status, "ready");
    LEARN_EXPECT_EQ(checks, latency, 2.5);

    const auto identity = std::tuple{id, name};
    const auto metrics = std::tuple{points, latency};
    const auto combined = std::tuple_cat(identity, metrics);
    LEARN_EXPECT_EQ(checks, std::get<2>(combined), 95);
    static_assert(std::tuple_size_v<decltype(combined)> == 4);

    const auto point = std::make_from_tuple<Point>(std::tuple{4, 9});
    LEARN_EXPECT_EQ(checks, point.x, 4);
    LEARN_EXPECT_EQ(checks, point.y, 9);

    constexpr auto version = std::pair{2, 3};
    static_assert(version < std::pair{2, 4});  // Lexicographical comparison.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_tuple_and_pair", run>;

}  // namespace
