// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : bit_cast_cpp20
// Topic id : part3/section04/bit_cast_cpp20
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <array>
#include <bit>
#include <concepts>
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part3/section04/bit_cast_cpp20";

struct Pair16 {
    std::uint16_t first;
    std::uint16_t second;
};
static_assert(std::is_trivially_copyable_v<Pair16>);
static_assert(sizeof(Pair16) == sizeof(std::uint32_t));

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr float one = 1.0F;
    constexpr auto bits = std::bit_cast<std::uint32_t>(one);
    LEARN_EXPECT_EQ(checks, std::bit_cast<float>(bits), one);
    const Pair16 input{0x1122U, 0x3344U};
    const auto packed = std::bit_cast<std::uint32_t>(input);
    const auto output = std::bit_cast<Pair16>(packed);
    LEARN_EXPECT_EQ(checks, output.first, input.first);
    LEARN_EXPECT_EQ(checks, output.second, input.second);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section04/bit_cast_cpp20", run>;

}  // namespace
