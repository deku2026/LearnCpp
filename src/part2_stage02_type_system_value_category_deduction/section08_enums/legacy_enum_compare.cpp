// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : legacy_enum_compare
// Topic id : part2/stage02/section08/legacy_enum_compare
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

enum LegacyColor {
    legacy_red = 1,
    legacy_green = 2,
};

enum class StrongColor {
    red = 1,
    green = 2,
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_convertible_v<LegacyColor, int>);
    static_assert(!std::is_convertible_v<StrongColor, int>);

    const int leaked_integer = legacy_green;
    learn::ExampleChecks checks{"part2/stage02/section08/legacy_enum_compare"};
    LEARN_EXPECT_EQ(checks, leaked_integer, 2);
    LEARN_EXPECT(checks, legacy_red != legacy_green);
    LEARN_EXPECT(checks, StrongColor::red != StrongColor::green);

    // Comparing unrelated unscoped enum types relies on their integer conversions.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/legacy_enum_compare", run>;

}  // namespace
