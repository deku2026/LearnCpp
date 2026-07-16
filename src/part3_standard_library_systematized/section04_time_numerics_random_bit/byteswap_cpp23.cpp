// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : byteswap_cpp23
// Topic id : part3/section04/byteswap_cpp23
// References: C++23 library clauses [utilities], [time], [numeric.limits], [bit], [charconv]

#include "learn/example_support.hpp"

#include <bit>
#include <concepts>
#include <cstdint>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section04/byteswap_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_byteswap) && __cpp_lib_byteswap >= 202110L
    ::learn::ExampleChecks checks{kTopic};
    constexpr std::uint64_t value = 0x0102030405060708ULL;
    constexpr auto swapped = std::byteswap(value);
    static_assert(swapped == 0x0807060504030201ULL);
    LEARN_EXPECT_EQ(checks, std::byteswap(swapped), value);
    LEARN_EXPECT_EQ(checks, std::byteswap(std::uint16_t{0x1234}), std::uint16_t{0x3412});
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::byteswap");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section04/byteswap_cpp23", run>;

}  // namespace
