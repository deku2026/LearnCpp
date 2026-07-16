// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : vector_iterator_invalidation
// Topic id : part2/stage10/section01/vector_iterator_invalidation
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section01/vector_iterator_invalidation";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<int> values{1, 2, 3, 4};
    auto next = values.erase(values.begin() + 1);  // The returned iterator is the safe continuation point.
    LEARN_EXPECT_EQ(checks, *next, 3);
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({1, 3, 4}));

    const std::size_t target = values.size();
    values.reserve(values.size() + 8);  // Treat all old iterators/references as invalid after reallocation.
    auto reacquired = values.begin() + static_cast<std::ptrdiff_t>(target - 1);
    LEARN_EXPECT_EQ(checks, *reacquired, 4);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section01/vector_iterator_invalidation", run>;

}  // namespace
