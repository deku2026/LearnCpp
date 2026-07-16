// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : basic_arithmetic_operators
// Topic id : part2/stage04/section05/basic_arithmetic_operators
// References: [over.binary], [over.match.oper]

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section05/basic_arithmetic_operators";

struct Vector2 {
    int x{};
    int y{};

    friend constexpr Vector2 operator+(Vector2 left, Vector2 right) noexcept {
        return {left.x + right.x, left.y + right.y};
    }
    friend constexpr Vector2 operator-(Vector2 left, Vector2 right) noexcept {
        return {left.x - right.x, left.y - right.y};
    }
    friend constexpr Vector2 operator*(Vector2 value, int scale) noexcept { return {value.x * scale, value.y * scale}; }
    friend constexpr bool operator==(Vector2, Vector2) = default;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    constexpr Vector2 first{3, 4};
    constexpr Vector2 second{1, 2};
    static_assert(first + second == Vector2{4, 6});
    static_assert((first - second) * 2 == Vector2{4, 4});

    LEARN_EXPECT_EQ(checks, first + second, (Vector2{4, 6}));
    LEARN_EXPECT_EQ(checks, first - first, (Vector2{0, 0}));

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section05/basic_arithmetic_operators", run>;

}  // namespace
