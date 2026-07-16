// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : zero_initialization_static_thread_local
// Topic id : part2/stage02/section02/zero_initialization_static_thread_local
//

#include "learn/example_support.hpp"

#include <cstddef>
#include <type_traits>

namespace {

int namespace_zero;
int* namespace_pointer;
thread_local int thread_zero;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static int function_zero;
    static_assert(std::is_trivially_default_constructible_v<int>);

    learn::ExampleChecks checks{"part2/stage02/section02/zero_initialization_static_thread_local"};
    LEARN_EXPECT_EQ(checks, namespace_zero, 0);
    LEARN_EXPECT(checks, namespace_pointer == nullptr);
    LEARN_EXPECT_EQ(checks, thread_zero, 0);
    LEARN_EXPECT_EQ(checks, function_zero, 0);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/zero_initialization_static_thread_local", run>;

}  // namespace
