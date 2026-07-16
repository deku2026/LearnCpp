// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section05_value_categories
// Item     : prvalue_materialization_cpp17
// Topic id : part2/stage02/section05/prvalue_materialization_cpp17
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

struct Box {
    int value;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_same_v<decltype((Box{1})), Box>);

    Box materialized = Box{7};
    const Box& lifetime_extended = Box{8};

    learn::ExampleChecks checks{"part2/stage02/section05/prvalue_materialization_cpp17"};
    LEARN_EXPECT_EQ(checks, materialized.value, 7);
    LEARN_EXPECT_EQ(checks, lifetime_extended.value, 8);

    // A prvalue has no identity until a context materializes its temporary object.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section05/prvalue_materialization_cpp17", run>;

}  // namespace
