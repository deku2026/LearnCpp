// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section06_class_misc
// Item     : literal_type_constexpr_constructor
// Topic id : part2/stage04/section06/literal_type_constexpr_constructor
// References: [basic.types], [dcl.constexpr]

#include "learn/example_support.hpp"

#include <cstdint>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section06/literal_type_constexpr_constructor";

struct Rgb {
    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;

    constexpr Rgb(std::uint8_t red_value, std::uint8_t green_value, std::uint8_t blue_value)
        : red(red_value), green(green_value), blue(blue_value) {}

    [[nodiscard]] constexpr std::uint32_t packed() const noexcept {
        return (static_cast<std::uint32_t>(red) << 16U) | (static_cast<std::uint32_t>(green) << 8U) |
               static_cast<std::uint32_t>(blue);
    }
};

static_assert(std::is_trivially_copyable_v<Rgb>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    constexpr Rgb orange{0xFF, 0x80, 0x00};
    static_assert(orange.packed() == 0xFF8000U);
    LEARN_EXPECT_EQ(checks, orange.red, std::uint8_t{0xFF});
    LEARN_EXPECT_EQ(checks, orange.packed(), std::uint32_t{0xFF8000});

    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage04/section06/literal_type_constexpr_constructor", run>;

}  // namespace
