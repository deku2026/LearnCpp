// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section04_iterators_and_invalidation
// Item     : stream_iterators
// Topic id : part2/stage10/section04/stream_iterators
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section04/stream_iterators";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::istringstream input{"10 20 bad 30"};
    const std::vector<int> values{std::istream_iterator<int>{input}, std::istream_iterator<int>{}};
    LEARN_EXPECT_EQ(checks, values, std::vector<int>({10, 20}));
    LEARN_EXPECT(checks, input.fail());

    std::ostringstream output;
    std::ranges::copy(values, std::ostream_iterator<int>{output, ","});
    LEARN_EXPECT_EQ(checks, output.str(), std::string{"10,20,"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section04/stream_iterators", run>;

}  // namespace
