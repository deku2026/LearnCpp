// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section01_sequence_containers
// Item     : std_array
// Topic id : part2/stage10/section01/std_array
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <array>
#include <span>
#include <stdexcept>
#include <string_view>
#include <tuple>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section01/std_array";

static_assert(std::tuple_size_v<std::array<int, 3>> == 3);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::array values{3, 1, 2};
    values.fill(7);
    values[1] = 8;
    const std::span<const int, 3> view{values};
    LEARN_EXPECT_EQ(checks, view.front(), 7);
    LEARN_EXPECT_EQ(checks, view[1], 8);
    LEARN_EXPECT_THROWS(checks, std::out_of_range, values.at(3));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section01/std_array", run>;

}  // namespace
