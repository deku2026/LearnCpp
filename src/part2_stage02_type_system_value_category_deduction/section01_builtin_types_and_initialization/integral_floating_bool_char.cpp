// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : integral_floating_bool_char
// Topic id : part2/stage02/section01/integral_floating_bool_char
//

#include "learn/example_support.hpp"

#include <limits>
#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_integral_v<int>);
    static_assert(std::is_floating_point_v<double>);
    static_assert(std::is_same_v<decltype(true), bool>);
    static_assert(sizeof(char) == 1);
    static_assert(std::numeric_limits<unsigned char>::digits >= 8);

    learn::ExampleChecks checks{"part2/stage02/section01/integral_floating_bool_char"};
    LEARN_EXPECT_EQ(checks, 7 / 2, 3);
    LEARN_EXPECT_EQ(checks, 7.0 / 2.0, 3.5);
    LEARN_EXPECT(checks, static_cast<bool>(42));
    LEARN_EXPECT(checks, !static_cast<bool>(0));

    // Plain char has implementation-defined signedness; never assume either result.
    LEARN_EXPECT(checks, std::numeric_limits<char>::is_signed || !std::numeric_limits<char>::is_signed);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/integral_floating_bool_char", run>;

}  // namespace
