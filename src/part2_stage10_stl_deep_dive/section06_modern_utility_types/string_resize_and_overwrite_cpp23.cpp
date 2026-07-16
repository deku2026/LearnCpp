// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_resize_and_overwrite_cpp23
// Topic id : part2/stage10/section06/string_resize_and_overwrite_cpp23

#ifdef __has_include
#if __has_include(<string>)
#include <string>
#define LEARN_HAS_STRING_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <charconv>
#include <cstdint>
#include <system_error>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/string_resize_and_overwrite_cpp23";

#if defined(LEARN_HAS_STRING_HEADER)

[[nodiscard]] std::string hex_fallback(std::uint32_t value) {
    std::string result(8, '\0');
    const auto converted = std::to_chars(result.data(), result.data() + result.size(), value, 16);
    if (converted.ec != std::errc{}) {
        return {};
    }
    result.resize(static_cast<std::size_t>(converted.ptr - result.data()));
    return result;
}

#if defined(__cpp_lib_string_resize_and_overwrite) && __cpp_lib_string_resize_and_overwrite >= 202110L
[[nodiscard]] std::string hex_direct(std::uint32_t value) {
    std::string result;
    result.resize_and_overwrite(8, [value](char* buffer, std::size_t capacity) noexcept {
        const auto converted = std::to_chars(buffer, buffer + capacity, value, 16);
        return converted.ec == std::errc{} ? static_cast<std::size_t>(converted.ptr - buffer) : 0U;
    });
    return result;
}
#endif

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_STRING_HEADER)
    learn::ExampleChecks checks{topic_id};
    LEARN_EXPECT_EQ(checks, hex_fallback(0xC0FFEEU), "c0ffee");

#if defined(__cpp_lib_string_resize_and_overwrite) && __cpp_lib_string_resize_and_overwrite >= 202110L
    LEARN_EXPECT_EQ(checks, hex_direct(0xC0FFEEU), "c0ffee");
    LEARN_EXPECT_EQ(checks, hex_direct(0U), "0");

    std::string generated;
    generated.resize_and_overwrite(10, [](char* data, std::size_t capacity) noexcept {
        const auto used = capacity / 2U;
        for (std::size_t index = 0; index < used; ++index) {
            data[index] = static_cast<char>('A' + index);
        }
        return used;  // The returned count becomes the final string size.
    });
    LEARN_EXPECT_EQ(checks, generated, "ABCDE");
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_string_resize_and_overwrite >= 202110L");
#endif
#else
    return learn::ExampleChecks::unavailable(topic_id, "standard <string> header");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/string_resize_and_overwrite_cpp23", run>;

}  // namespace
