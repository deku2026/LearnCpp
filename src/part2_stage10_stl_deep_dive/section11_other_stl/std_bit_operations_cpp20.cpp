// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_bit_operations_cpp20
// Topic id : part2/stage10/section11/std_bit_operations_cpp20
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <bit>
#include <cstdint>
#include <limits>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section11/std_bit_operations_cpp20";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr std::uint8_t value = 0b00110100U;
    static_assert(std::popcount(value) == 3);
    LEARN_EXPECT_EQ(checks, std::countl_zero(value), 2);
    LEARN_EXPECT_EQ(checks, std::countr_zero(value), 2);
    LEARN_EXPECT_EQ(checks, std::bit_width(std::uint8_t{16}), 5);
    LEARN_EXPECT_EQ(checks, std::bit_floor(std::uint8_t{19}), std::uint8_t{16});
    LEARN_EXPECT_EQ(checks, std::bit_ceil(std::uint8_t{19}), std::uint8_t{32});
    LEARN_EXPECT_EQ(checks, std::rotl(std::uint8_t{1}, 2), std::uint8_t{4});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section11/std_bit_operations_cpp20", run>;

}  // namespace
