// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : bit_operations_cpp20
// Topic id : part3/section04/bit_operations_cpp20
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <bit>
#include <concepts>
#include <cstdint>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section04/bit_operations_cpp20";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, std::popcount(std::uint32_t{0b101101}), 4);
    LEARN_EXPECT_EQ(checks, std::countl_zero(std::uint8_t{1}), 7);
    LEARN_EXPECT_EQ(checks, std::countr_zero(std::uint8_t{8}), 3);
    LEARN_EXPECT(checks, std::has_single_bit(std::uint32_t{64}));
    LEARN_EXPECT_EQ(checks, std::bit_width(std::uint32_t{0}), 0);
    LEARN_EXPECT_EQ(checks, std::bit_floor(std::uint32_t{70}), 64U);
    LEARN_EXPECT_EQ(checks, std::bit_ceil(std::uint32_t{70}), 128U);
    LEARN_EXPECT_EQ(checks, std::rotr(std::uint8_t{1}, 1), std::uint8_t{0x80});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section04/bit_operations_cpp20", run>;

}  // namespace
