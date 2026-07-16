// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section04_compound_types
// Item     : rvalue_reference
// Topic id : part2/stage02/section04/rvalue_reference
//

#include "learn/example_support.hpp"

#include <type_traits>
#include <utility>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    int&& reference = 41;
    static_assert(std::is_same_v<decltype(reference), int&&>);
    static_assert(std::is_same_v<decltype((reference)), int&>);
    static_assert(std::is_same_v<decltype(std::move(reference)), int&&>);

    reference += 1;  // A named rvalue reference expression is an lvalue.
    int copied_from_xvalue = std::move(reference);

    learn::ExampleChecks checks{"part2/stage02/section04/rvalue_reference"};
    LEARN_EXPECT_EQ(checks, reference, 42);
    LEARN_EXPECT_EQ(checks, copied_from_xvalue, 42);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section04/rvalue_reference", run>;

}  // namespace
