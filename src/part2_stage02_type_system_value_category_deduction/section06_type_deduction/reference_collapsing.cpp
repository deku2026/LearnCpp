// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : reference_collapsing
// Topic id : part2/stage02/section06/reference_collapsing
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

template <class T>
using LvalueReference = T&;

template <class T>
using RvalueReference = T&&;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_same_v<LvalueReference<int&>, int&>);
    static_assert(std::is_same_v<LvalueReference<int&&>, int&>);
    static_assert(std::is_same_v<RvalueReference<int&>, int&>);
    static_assert(std::is_same_v<RvalueReference<int&&>, int&&>);

    int value = 5;
    RvalueReference<int&> collapsed = value;

    learn::ExampleChecks checks{"part2/stage02/section06/reference_collapsing"};
    collapsed = 8;
    LEARN_EXPECT_EQ(checks, value, 8);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/reference_collapsing", run>;

}  // namespace
