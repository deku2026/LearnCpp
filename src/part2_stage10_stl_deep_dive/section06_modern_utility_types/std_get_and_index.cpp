// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : std_get_and_index
// Topic id : part2/stage10/section06/std_get_and_index

#include "learn/example_support.hpp"

#include <array>
#include <string>
#include <tuple>
#include <utility>
#include <variant>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/std_get_and_index";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    std::tuple<int, std::string, double> record{7, "Ada", 9.5};
    std::get<0>(record) += 1;
    std::get<std::string>(record) += " Lovelace";  // Type form requires a unique matching type.
    LEARN_EXPECT_EQ(checks, std::get<0>(record), 8);
    LEARN_EXPECT_EQ(checks, std::get<1>(record), "Ada Lovelace");

    std::pair<int, std::string> key_value{3, "three"};
    LEARN_EXPECT_EQ(checks, std::get<int>(key_value), 3);
    LEARN_EXPECT_EQ(checks, std::get<1>(key_value), "three");

    constexpr std::array digits{2, 4, 8, 16};
    static_assert(std::get<2>(digits) == 8);
    static_assert(std::tuple_size_v<decltype(record)> == 3);

    std::variant<int, std::string> result = std::string{"ready"};
    LEARN_EXPECT_EQ(checks, result.index(), 1U);
    LEARN_EXPECT_EQ(checks, std::get<std::string>(result), "ready");
    LEARN_EXPECT_THROWS(checks, std::bad_variant_access, std::get<0>(result));
    result = 99;
    LEARN_EXPECT_EQ(checks, std::get<0>(result), 99);
    static_assert(std::variant_size_v<decltype(result)> == 2);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_get_and_index", run>;

}  // namespace
