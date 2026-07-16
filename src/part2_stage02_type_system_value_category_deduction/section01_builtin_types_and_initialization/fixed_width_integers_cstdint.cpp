// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : fixed_width_integers_cstdint
// Topic id : part2/stage02/section01/fixed_width_integers_cstdint
//

#include "learn/example_support.hpp"

#include <cstdint>
#include <limits>
#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::numeric_limits<std::uint_least32_t>::digits >= 32);
    static_assert(std::is_signed_v<std::int_least32_t>);
    static_assert(std::is_unsigned_v<std::uint_least32_t>);

    learn::ExampleChecks checks{"part2/stage02/section01/fixed_width_integers_cstdint"};
    const std::uint_least32_t mask = UINT32_C(0xFF00);
    LEARN_EXPECT_EQ(checks, mask & UINT32_C(0x0F00), UINT32_C(0x0F00));

    const auto maximum = std::numeric_limits<std::uint_least32_t>::max();
    LEARN_EXPECT_EQ(checks, static_cast<std::uint_least32_t>(maximum + 1U), 0U);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/fixed_width_integers_cstdint", run>;

}  // namespace
