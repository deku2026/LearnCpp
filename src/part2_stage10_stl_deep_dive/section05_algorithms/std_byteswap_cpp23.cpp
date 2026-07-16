// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section05_algorithms
// Item     : std_byteswap_cpp23
// Topic id : part2/stage10/section05/std_byteswap_cpp23
// References: C++23 [container.requirements], [algorithms], [iterator.concepts], [numeric.ops]

#include "learn/example_support.hpp"

#include <bit>
#include <cstdint>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section05/std_byteswap_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
    ::learn::ExampleChecks checks{kTopic};
    constexpr std::uint32_t value = 0x11223344U;
    constexpr auto swapped = std::byteswap(value);
    static_assert(swapped == 0x44332211U);
    LEARN_EXPECT_EQ(checks, std::byteswap(swapped), value);
    constexpr std::uint8_t byte = 0xABU;
    LEARN_EXPECT_EQ(checks, std::byteswap(byte), byte);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::byteswap");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section05/std_byteswap_cpp23", run>;

}  // namespace
