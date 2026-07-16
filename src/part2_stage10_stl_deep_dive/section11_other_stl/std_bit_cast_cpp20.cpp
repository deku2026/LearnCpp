// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_bit_cast_cpp20
// Topic id : part2/stage10/section11/std_bit_cast_cpp20
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <bit>
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section11/std_bit_cast_cpp20";

struct Bytes {
    std::uint16_t low;
    std::uint16_t high;
};

static_assert(std::is_trivially_copyable_v<Bytes>);
static_assert(sizeof(Bytes) == sizeof(std::uint32_t));

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr float value = 1.0F;
    constexpr std::uint32_t bits = std::bit_cast<std::uint32_t>(value);
    LEARN_EXPECT_EQ(checks, std::bit_cast<float>(bits), value);
    // bit_cast copies the object representation; it is not a numeric conversion or aliasing cast.
    const Bytes bytes{0x1122U, 0x3344U};
    const auto representation = std::bit_cast<std::uint32_t>(bytes);
    LEARN_EXPECT_EQ(checks, std::bit_cast<Bytes>(representation).high, bytes.high);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section11/std_bit_cast_cpp20", run>;

}  // namespace
