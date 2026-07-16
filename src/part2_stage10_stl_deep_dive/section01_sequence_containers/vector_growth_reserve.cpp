// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : vector_growth_reserve
// Topic id : part2/stage10/section01/vector_growth_reserve
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section01/vector_growth_reserve";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values;
    values.reserve(4);
    const auto reserved = values.capacity();
    values.push_back(1);
    const int* first_address = values.data();
    values.push_back(2);
    values.push_back(3);
    LEARN_EXPECT(checks, values.data() == first_address);
    LEARN_EXPECT(checks, values.capacity() >= reserved);
    LEARN_EXPECT_EQ(checks, values.size(), 3U);
    values.shrink_to_fit();  // Non-binding request: never assert that capacity changed.
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({1, 2, 3}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section01/vector_growth_reserve", run>;

}  // namespace
