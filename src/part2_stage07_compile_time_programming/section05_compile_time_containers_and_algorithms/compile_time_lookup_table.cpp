// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : compile_time_lookup_table
// Topic id : part2/stage07/section05/compile_time_lookup_table
// References: N4950 [expr.const], [array].

#include "learn/example_support.hpp"

#include <array>
#include <cstdint>
#include <string_view>

namespace {

constexpr std::uint32_t crc32_entry(std::uint32_t value) noexcept {
    for (int bit = 0; bit < 8; ++bit) {
        const bool low_bit_is_set = (value & 1U) != 0;
        value = (value >> 1U) ^ (low_bit_is_set ? 0xEDB8'8320U : 0U);
    }
    return value;
}

constexpr auto make_crc32_table() noexcept {
    std::array<std::uint32_t, 256> table{};
    for (std::uint32_t index = 0; index < table.size(); ++index) {
        table[index] = crc32_entry(index);
    }
    return table;
}

inline constexpr auto crc32_table = make_crc32_table();

constexpr std::uint32_t crc32(std::string_view text) noexcept {
    std::uint32_t state = 0xFFFF'FFFFU;
    for (const char character : text) {
        const auto byte = static_cast<unsigned char>(character);
        const auto table_index = static_cast<unsigned char>(state ^ byte);
        state = crc32_table[table_index] ^ (state >> 8U);
    }
    return state ^ 0xFFFF'FFFFU;
}

static_assert(crc32_table[0] == 0x0000'0000U);
static_assert(crc32_table[1] == 0x7707'3096U);
static_assert(crc32("") == 0x0000'0000U);
static_assert(crc32("123456789") == 0xCBF4'3926U);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage07/section05/compile_time_lookup_table"};

    LEARN_EXPECT_EQ(checks, crc32_table.size(), 256uz);
    LEARN_EXPECT_EQ(checks, crc32("123456789"), 0xCBF4'3926U);
    LEARN_EXPECT_EQ(checks, crc32("LearnCpp"), 0x543B'BFBAU);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/compile_time_lookup_table", run>;

}  // namespace
