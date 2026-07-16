// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section04_compound_types
// Item     : lvalue_reference
// Topic id : part2/stage02/section04/lvalue_reference
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    int value = 3;
    int& alias = value;
    const int& lifetime_extended = 40 + 2;

    static_assert(std::is_lvalue_reference_v<decltype(alias)>);
    static_assert(std::is_same_v<decltype(lifetime_extended), const int&>);

    alias = 8;
    learn::ExampleChecks checks{"part2/stage02/section04/lvalue_reference"};
    LEARN_EXPECT(checks, &alias == &value);
    LEARN_EXPECT_EQ(checks, value, 8);
    LEARN_EXPECT_EQ(checks, lifetime_extended, 42);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section04/lvalue_reference", run>;

}  // namespace
