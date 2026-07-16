// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : views_elements_keys_values
// Topic id : part2/stage10/section07/views_elements_keys_values
//

#include "learn/example_support.hpp"

#include <map>
#include <ranges>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/views_elements_keys_values"};
    std::vector<std::tuple<int, std::string, double>> records{{1, "Ada", 9.5}, {2, "Grace", 8.5}};
    auto ids = records | std::views::elements<0>;
    auto names = records | std::views::elements<1>;
    auto scores = records | std::views::elements<2>;
    static_assert(std::is_same_v<std::ranges::range_reference_t<decltype(names)>, std::string&>);

    std::vector<int> observed_ids;
    for (const int id : ids) {
        observed_ids.push_back(id);
    }
    for (std::string& name : names) {
        name += '!';
    }

    double score_total = 0.0;
    for (const double score : scores) {
        score_total += score;
    }

    std::map<std::string, int> inventory{{"apple", 2}, {"pear", 3}};
    auto keys = inventory | std::views::keys;
    auto values = inventory | std::views::values;
    static_assert(std::is_const_v<std::remove_reference_t<std::ranges::range_reference_t<decltype(keys)>>>);
    for (int& value : values) {
        value *= 10;
    }

    std::vector<std::string> observed_keys;
    for (const std::string& key : keys) {
        observed_keys.push_back(key);
    }

    LEARN_EXPECT_EQ(checks, observed_ids, (std::vector{1, 2}));
    LEARN_EXPECT_EQ(checks, std::get<1>(records.front()), std::string{"Ada!"});
    LEARN_EXPECT_EQ(checks, score_total, 18.0);
    LEARN_EXPECT_EQ(checks, observed_keys, (std::vector<std::string>{"apple", "pear"}));
    LEARN_EXPECT_EQ(checks, inventory.at("apple"), 20);
    LEARN_EXPECT_EQ(checks, inventory.at("pear"), 30);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/views_elements_keys_values", run>;

}  // namespace
