// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : pipe_drop_while
// Topic id : part2/stage10/section07/pipe_drop_while
//

#include "learn/example_support.hpp"

#include <ranges>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/pipe_drop_while"};
    std::vector values{1, 2, 3, 0, 4};
    int predicate_calls = 0;
    auto tail = values | std::views::drop_while([&predicate_calls](const int value) {
                    ++predicate_calls;
                    return value < 3;
                });

    LEARN_EXPECT_EQ(checks, predicate_calls, 0);
    std::vector<int> observed;
    for (const int value : tail) {
        observed.push_back(value);
    }

    LEARN_EXPECT_EQ(checks, observed, (std::vector{3, 0, 4}));
    LEARN_EXPECT_EQ(checks, predicate_calls, 3);
    // Once 3 fails the predicate, later values are yielded without further tests.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/pipe_drop_while", run>;

}  // namespace
