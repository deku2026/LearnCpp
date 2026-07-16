// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section05_value_categories
// Item     : lvalue_xvalue_prvalue
// Topic id : part2/stage02/section05/lvalue_xvalue_prvalue
//

#include "learn/example_support.hpp"

#include <type_traits>
#include <utility>

namespace {

int make_value() {
    return 6;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    int value = 5;
    static_assert(std::is_same_v<decltype((value)), int&>);              // lvalue
    static_assert(std::is_same_v<decltype((std::move(value))), int&&>);  // xvalue
    static_assert(std::is_same_v<decltype((make_value())), int>);        // prvalue

    int& lvalue_alias = value;
    int&& xvalue_alias = std::move(value);
    int from_prvalue = make_value();

    learn::ExampleChecks checks{"part2/stage02/section05/lvalue_xvalue_prvalue"};
    LEARN_EXPECT(checks, &lvalue_alias == &value);
    LEARN_EXPECT(checks, &xvalue_alias == &value);
    LEARN_EXPECT_EQ(checks, from_prvalue, 6);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section05/lvalue_xvalue_prvalue", run>;

}  // namespace
