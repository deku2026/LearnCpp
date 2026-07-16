// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section03_const_constexpr_volatile
// Item     : constexpr_variable_intro
// Topic id : part2/stage02/section03/constexpr_variable_intro
//

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <type_traits>

namespace {

constexpr int square(int value) {
    return value * value;
}

int run(int argc, char** argv) {
    (void)argv;

    constexpr int extent = square(3);
    std::array<int, extent> values{};
    static_assert(extent == 9);
    static_assert(values.size() == 9);

    const int runtime_const = argc;  // const does not imply a constant expression.
    learn::ExampleChecks checks{"part2/stage02/section03/constexpr_variable_intro"};
    LEARN_EXPECT_EQ(checks, values.size(), std::size_t{9});
    LEARN_EXPECT(checks, runtime_const >= 0);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section03/constexpr_variable_intro", run>;

}  // namespace
