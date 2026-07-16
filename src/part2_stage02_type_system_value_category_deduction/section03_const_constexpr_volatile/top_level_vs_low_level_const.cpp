// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section03_const_constexpr_volatile
// Item     : top_level_vs_low_level_const
// Topic id : part2/stage02/section03/top_level_vs_low_level_const
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    const int source = 10;
    auto top_level_copy = source;
    auto& preserved = source;

    int mutable_value = 11;
    const int* low_level_const = &mutable_value;
    int* const top_level_const_pointer = &mutable_value;

    static_assert(std::is_same_v<decltype(top_level_copy), int>);
    static_assert(std::is_same_v<decltype(preserved), const int&>);
    static_assert(std::is_same_v<decltype(low_level_const), const int*>);
    static_assert(std::is_same_v<decltype(top_level_const_pointer), int* const>);

    *top_level_const_pointer = 12;
    learn::ExampleChecks checks{"part2/stage02/section03/top_level_vs_low_level_const"};
    LEARN_EXPECT_EQ(checks, top_level_copy, 10);
    LEARN_EXPECT_EQ(checks, *low_level_const, 12);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section03/top_level_vs_low_level_const", run>;

}  // namespace
