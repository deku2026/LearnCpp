// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section05_value_categories
// Item     : glvalue_and_rvalue
// Topic id : part2/stage02/section05/glvalue_and_rvalue
//

#include "learn/example_support.hpp"

#include <type_traits>
#include <utility>

namespace {

int& lvalue_result(int& value) {
    return value;
}

int prvalue_result() {
    return 17;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    int object = 9;
    static_assert(std::is_same_v<decltype((object)), int&>);
    static_assert(std::is_same_v<decltype((lvalue_result(object))), int&>);
    static_assert(std::is_same_v<decltype((std::move(object))), int&&>);
    static_assert(std::is_same_v<decltype((prvalue_result())), int>);

    lvalue_result(object) = 12;
    learn::ExampleChecks checks{"part2/stage02/section05/glvalue_and_rvalue"};
    LEARN_EXPECT_EQ(checks, object, 12);
    LEARN_EXPECT_EQ(checks, prvalue_result(), 17);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section05/glvalue_and_rvalue", run>;

}  // namespace
