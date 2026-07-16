// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : auto_decay_copy_cpp23
// Topic id : part2/stage02/section06/auto_decay_copy_cpp23
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_auto_cast) && __cpp_auto_cast >= 202110L
    int values[3]{1, 2, 3};
    const int constant = 7;
    auto decayed = auto(values);
    auto copied = auto(constant);

    static_assert(std::is_same_v<decltype(decayed), int*>);
    static_assert(std::is_same_v<decltype(copied), int>);

    learn::ExampleChecks checks{"part2/stage02/section06/auto_decay_copy_cpp23"};
    LEARN_EXPECT_EQ(checks, decayed[2], 3);
    copied = 9;
    LEARN_EXPECT_EQ(checks, constant, 7);
    LEARN_EXPECT_EQ(checks, copied, 9);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage02/section06/auto_decay_copy_cpp23", "__cpp_auto_cast");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/auto_decay_copy_cpp23", run>;

}  // namespace
