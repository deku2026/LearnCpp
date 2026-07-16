// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : comparison_operators_cpp20_spaceship
// Topic id : part2/stage04/section05/comparison_operators_cpp20_spaceship
// References: [class.spaceship], [over.match.oper]

#include "learn/example_support.hpp"

#include <compare>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section05/comparison_operators_cpp20_spaceship";

struct SemanticVersion {
    int major{};
    int minor{};
    int patch{};
    auto operator<=>(const SemanticVersion&) const = default;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    constexpr SemanticVersion old_version{1, 9, 9};
    constexpr SemanticVersion new_version{2, 0, 0};
    constexpr SemanticVersion same{2, 0, 0};

    static_assert(old_version < new_version);
    static_assert(new_version == same);
    LEARN_EXPECT(checks, new_version >= old_version);
    LEARN_EXPECT(checks, (new_version <=> same) == std::strong_ordering::equal);

    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage04/section05/comparison_operators_cpp20_spaceship", run>;

}  // namespace
