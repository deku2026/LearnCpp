// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section01_language_basics
// Item     : builtin_types_and_literals
// Topic id : part2/stage01/section01/builtin_types_and_literals
// References: N4950 [basic.fundamental], [lex.literal], [conv.ptr]; P0330R8.

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string_view>
#include <type_traits>

namespace {

enum class SelectedOverload { integer, pointer };

constexpr SelectedOverload select_overload(int) noexcept {
    return SelectedOverload::integer;
}

constexpr SelectedOverload select_overload(const void*) noexcept {
    return SelectedOverload::pointer;
}

static_assert(sizeof(char) == 1);
static_assert(sizeof(short) <= sizeof(int));
static_assert(sizeof(int) <= sizeof(long));
static_assert(sizeof(long) <= sizeof(long long));
static_assert(std::numeric_limits<std::uint_least32_t>::digits >= 32);

static_assert(std::is_same_v<decltype(42), int>);
static_assert(std::is_same_v<decltype(42U), unsigned int>);
static_assert(std::is_same_v<decltype(42LL), long long>);
static_assert(std::is_same_v<decltype(0uz), std::size_t>);
static_assert(std::is_same_v<decltype(nullptr), std::nullptr_t>);
static_assert(std::is_same_v<decltype(u8'x'), char8_t>);

static_assert(42 == 0x2A);
static_assert(42 == 052);
static_assert(42 == 0b10'1010);
static_assert(select_overload(0) == SelectedOverload::integer);
static_assert(select_overload(nullptr) == SelectedOverload::pointer);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section01/builtin_types_and_literals"};

    constexpr std::array values{1'000'000, 0x0F'F0, 0b1010'0101};
    LEARN_EXPECT_EQ(checks, values[0], 1000000);
    LEARN_EXPECT_EQ(checks, values[1], 4080);
    LEARN_EXPECT_EQ(checks, values[2], 165);

    constexpr std::string_view adjacent =
        "translation "
        "unit";
    constexpr std::string_view raw = R"(C:\study\stage1)";
    LEARN_EXPECT_EQ(checks, adjacent, std::string_view{"translation unit"});
    LEARN_EXPECT_EQ(checks, raw, std::string_view{R"(C:\study\stage1)"});

    int value_initialized{};
    LEARN_EXPECT_EQ(checks, value_initialized, 0);

    const bool observed_char_signedness = static_cast<char>(-1) < 0;
    LEARN_EXPECT_EQ(checks, observed_char_signedness, std::numeric_limits<char>::is_signed);

    // NULL can be an integer macro. nullptr has its own type and selects pointer overloads.
    LEARN_EXPECT_EQ(checks, select_overload(0), SelectedOverload::integer);
    LEARN_EXPECT_EQ(checks, select_overload(nullptr), SelectedOverload::pointer);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/builtin_types_and_literals", run>;

}  // namespace
