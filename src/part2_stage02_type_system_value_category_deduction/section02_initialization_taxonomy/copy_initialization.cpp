// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : copy_initialization
// Topic id : part2/stage02/section02/copy_initialization
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

struct Distance {
    Distance(double input) : value(input) {}
    double value;
};

struct ExplicitDistance {
    explicit ExplicitDistance(double input) : value(input) {}
    double value;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_convertible_v<double, Distance>);
    static_assert(!std::is_convertible_v<double, ExplicitDistance>);
    static_assert(std::is_constructible_v<ExplicitDistance, double>);

    Distance implicit = 2.5;
    ExplicitDistance explicit_only{2.5};
    int original = 8;
    int copy = original;

    learn::ExampleChecks checks{"part2/stage02/section02/copy_initialization"};
    LEARN_EXPECT_EQ(checks, implicit.value, 2.5);
    LEARN_EXPECT_EQ(checks, explicit_only.value, 2.5);
    copy = 9;
    LEARN_EXPECT_EQ(checks, original, 8);
    LEARN_EXPECT_EQ(checks, copy, 9);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/copy_initialization", run>;

}  // namespace
