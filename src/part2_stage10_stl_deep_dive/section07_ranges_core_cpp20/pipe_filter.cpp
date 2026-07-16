// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_filter
// Topic id : part2/stage10/section07/pipe_filter
//

#include "learn/example_support.hpp"

#include <ranges>
#include <type_traits>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/pipe_filter"};
    std::vector values{1, 2, 3, 4, 5, 6};
    int predicate_calls = 0;
    auto evens = values | std::views::filter([&predicate_calls](const int value) {
                     ++predicate_calls;
                     return value % 2 == 0;
                 });
    static_assert(std::is_same_v<std::ranges::range_reference_t<decltype(evens)>, int&>);

    LEARN_EXPECT_EQ(checks, predicate_calls, 0);
    std::vector<int> observed;
    for (const int value : evens) {
        observed.push_back(value);
    }

    LEARN_EXPECT_EQ(checks, observed, (std::vector{2, 4, 6}));
    LEARN_EXPECT_EQ(checks, predicate_calls, 6);

    auto greater_than_two = values | std::views::filter([](const int value) { return value > 2; });
    *greater_than_two.begin() = 30;
    LEARN_EXPECT_EQ(checks, values[2], 30);

#if 0
    auto invalidated = evens.begin();
    values.reserve(values.capacity() + 1);  // Reallocation invalidates the cached iterator.
    const int undefined = *invalidated;
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_filter", run>;

}  // namespace
