// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section03_const_constexpr_volatile
// Item     : const_qualifier_pointer_reference
// Topic id : part2/stage02/section03/const_qualifier_pointer_reference
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    int value = 5;
    const int* pointer_to_const = &value;
    int* const const_pointer = &value;
    const int* const const_pointer_to_const = &value;
    const int& const_reference = value;

    static_assert(std::is_same_v<decltype(pointer_to_const), const int*>);
    static_assert(std::is_same_v<decltype(const_pointer), int* const>);
    static_assert(std::is_same_v<decltype(const_pointer_to_const), const int* const>);
    static_assert(std::is_same_v<decltype(const_reference), const int&>);

    *const_pointer = 8;
    learn::ExampleChecks checks{"part2/stage02/section03/const_qualifier_pointer_reference"};
    LEARN_EXPECT_EQ(checks, *pointer_to_const, 8);
    LEARN_EXPECT_EQ(checks, const_reference, 8);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section03/const_qualifier_pointer_reference", run>;

}  // namespace
