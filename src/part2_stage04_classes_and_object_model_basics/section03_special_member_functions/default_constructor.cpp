// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : default_constructor
// Topic id : part2/stage04/section03/default_constructor
// References: [class.default.ctor]

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section03/default_constructor";

struct Coordinates {
    int x{};
    int y{};

    Coordinates() = default;
    explicit constexpr Coordinates(int both) : x(both), y(both) {}
};

static_assert(std::is_default_constructible_v<Coordinates>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Coordinates origin;
    constexpr Coordinates diagonal{4};

    LEARN_EXPECT_EQ(checks, origin.x, 0);
    LEARN_EXPECT_EQ(checks, origin.y, 0);
    static_assert(diagonal.x == 4 && diagonal.y == 4);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section03/default_constructor", run>;

}  // namespace
