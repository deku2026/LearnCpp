// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : auto_with_initialization
// Topic id : part2/stage02/section01/auto_with_initialization
//

#include "learn/example_support.hpp"

#include <cstddef>
#include <initializer_list>
#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    const int source = 7;
    auto copied = source;
    auto& aliased = source;
    auto values = {1, 2, 3};
    auto scalar{1};

    static_assert(std::is_same_v<decltype(copied), int>);
    static_assert(std::is_same_v<decltype(aliased), const int&>);
    static_assert(std::is_same_v<decltype(values), std::initializer_list<int>>);
    static_assert(std::is_same_v<decltype(scalar), int>);

    learn::ExampleChecks checks{"part2/stage02/section01/auto_with_initialization"};
    copied = 9;
    LEARN_EXPECT_EQ(checks, source, 7);
    LEARN_EXPECT_EQ(checks, copied, 9);
    LEARN_EXPECT_EQ(checks, values.size(), std::size_t{3});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/auto_with_initialization", run>;

}  // namespace
