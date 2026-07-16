// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_sort
// Topic id : part2/stage10/section05/std_sort
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/std_sort";

struct Record {
    std::string name;
    int score;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<Record> records{{"Lin", 8}, {"Ada", 10}, {"Kai", 8}};
    std::ranges::sort(records, std::ranges::greater{}, &Record::score);
    LEARN_EXPECT_EQ(checks, records.front().name, std::string{"Ada"});
    LEARN_EXPECT(checks, std::ranges::is_sorted(records, std::ranges::greater{}, &Record::score));
    std::ranges::stable_sort(records, {}, &Record::name);
    LEARN_EXPECT_EQ(checks, records[1].name, std::string{"Kai"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section05/std_sort", run>;

}  // namespace
