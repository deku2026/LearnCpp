// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : decltype_auto
// Topic id : part2/stage02/section06/decltype_auto
//

#include "learn/example_support.hpp"

#include <type_traits>
#include <utility>

namespace {

decltype(auto) preserve_reference(int& value) {
    return (value);
}

auto copy_value(int& value) {
    return value;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_same_v<decltype(preserve_reference(std::declval<int&>())), int&>);
    static_assert(std::is_same_v<decltype(copy_value(std::declval<int&>())), int>);

    int value = 3;
    preserve_reference(value) = 9;
    int copy = copy_value(value);
    copy = 10;

    learn::ExampleChecks checks{"part2/stage02/section06/decltype_auto"};
    LEARN_EXPECT_EQ(checks, value, 9);
    LEARN_EXPECT_EQ(checks, copy, 10);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/decltype_auto", run>;

}  // namespace
