// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : defaulted_comparison_operators
// Topic id : part2/stage04/section05/defaulted_comparison_operators
// References: [class.eq], [class.spaceship]

#include "learn/example_support.hpp"

#include <compare>
#include <string>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section05/defaulted_comparison_operators";

struct Key {
    std::string group;
    int sequence{};
    auto operator<=>(const Key&) const = default;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const Key first{"alpha", 2};
    const Key same{"alpha", 2};
    const Key later{"alpha", 3};
    const Key next_group{"beta", 0};

    LEARN_EXPECT(checks, first == same);
    LEARN_EXPECT(checks, first != later);
    LEARN_EXPECT(checks, first < later);
    LEARN_EXPECT(checks, later < next_group);  // Members compare in declaration order.

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section05/defaulted_comparison_operators", run>;

}  // namespace
