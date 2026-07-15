// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : views_elements_keys_values
// Topic id : part2/stage10/section07_ranges_core_cpp20/views_elements_keys_values
//
// Covers: views::elements / keys / values

#include "learn/topic_registry.hpp"

#include <map>
#include <ranges>
#include <string>
#include <vector>

namespace {

void demo_basics() {
    std::map<std::string, int> m{{"a", 1}, {"b", 2}};
    auto keys = m | std::views::keys;
    std::vector<std::string> k(keys.begin(), keys.end());
    LEARN_CHECK(k.size() == 2);
    LEARN_CHECK(k[0] == "a");
}

void demo_intermediate() {
    std::map<int, int> m{{1, 10}, {2, 20}};
    auto vals = m | std::views::values;
    int sum = 0;
    for (int x : vals) {
        sum += x;
    }
    LEARN_CHECK(sum == 30);
}

void demo_expert() {
    std::vector<std::tuple<int, char, double>> v{{1, 'a', 1.5}, {2, 'b', 2.5}};
    auto mid = v | std::views::elements<1>;
    std::vector<char> out(mid.begin(), mid.end());
    LEARN_CHECK((out == std::vector<char>{'a', 'b'}));
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section07_ranges_core_cpp20/views_elements_keys_values", run>;

}  // namespace
