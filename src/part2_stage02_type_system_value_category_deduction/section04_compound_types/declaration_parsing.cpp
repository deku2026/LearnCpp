// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section04_compound_types
// Item     : declaration_parsing
// Topic id : part2/stage02/section04/declaration_parsing
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int add_one(int value) {
    return value + 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    int value = 4;
    int* pointer = &value;
    int second = 5;  // In int* pointer, second; only pointer is a pointer.
    int array[3]{1, 2, 3};
    int (*pointer_to_array)[3] = &array;
    using FunctionPointer = int (*)(int);
    FunctionPointer function = &add_one;

    static_assert(std::is_same_v<decltype(pointer), int*>);
    static_assert(std::is_same_v<decltype(second), int>);
    static_assert(std::is_same_v<decltype(pointer_to_array), int (*)[3]>);

    learn::ExampleChecks checks{"part2/stage02/section04/declaration_parsing"};
    LEARN_EXPECT_EQ(checks, *pointer, 4);
    LEARN_EXPECT_EQ(checks, (*pointer_to_array)[2], 3);
    LEARN_EXPECT_EQ(checks, function(second), 6);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section04/declaration_parsing", run>;

}  // namespace
