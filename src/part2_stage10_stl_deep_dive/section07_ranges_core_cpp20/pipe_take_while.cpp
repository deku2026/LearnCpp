// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_take_while
// Topic id : part2/stage10/section07/pipe_take_while
//

#include "learn/example_support.hpp"

#include <ranges>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/pipe_take_while"};
    std::vector values{2, 4, 6, 7, 8};
    int predicate_calls = 0;
    auto even_prefix = values | std::views::take_while([&predicate_calls](const int value) {
                           ++predicate_calls;
                           return value % 2 == 0;
                       });
    static_assert(!std::ranges::common_range<decltype(even_prefix)>);

    LEARN_EXPECT_EQ(checks, predicate_calls, 0);
    std::vector<int> observed;
    for (const int value : even_prefix) {
        observed.push_back(value);
    }

    LEARN_EXPECT_EQ(checks, observed, (std::vector{2, 4, 6}));
    LEARN_EXPECT_EQ(checks, predicate_calls, 4);
    // 7 is tested but excluded; iteration stops there, so 8 is never tested.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_take_while", run>;

}  // namespace
