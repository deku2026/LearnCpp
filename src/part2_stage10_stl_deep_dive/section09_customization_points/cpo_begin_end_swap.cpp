// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section09_customization_points
// Item     : cpo_begin_end_swap
// Topic id : part2/stage10/section09/cpo_begin_end_swap
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section09/cpo_begin_end_swap";

struct Buffer {
    int values[3]{};
    int* begin() noexcept { return values; }
    int* end() noexcept { return values + 3; }
    const int* begin() const noexcept { return values; }
    const int* end() const noexcept { return values + 3; }

    friend void swap(Buffer& left, Buffer& right) noexcept { std::ranges::swap_ranges(left.values, right.values); }
};

static_assert(std::ranges::range<Buffer>);
static_assert(std::swappable<Buffer>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Buffer left{{1, 2, 3}};
    Buffer right{{4, 5, 6}};
    LEARN_EXPECT_EQ(checks, *std::ranges::begin(left), 1);
    std::ranges::swap(left, right);  // The CPO finds the hidden-friend customization.
    LEARN_EXPECT_EQ(checks, left.values[0], 4);
    int built_in[]{7, 8};
    LEARN_EXPECT_EQ(checks, *std::ranges::begin(built_in), 7);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section09/cpo_begin_end_swap", run>;

}  // namespace
