// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : auto_deduces_decays
// Topic id : part2/stage02/section06/auto_deduces_decays
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

int sample_function(double value) {
    return static_cast<int>(value);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    const int constant = 5;
    const int& reference = constant;
    int array[2]{1, 2};

    auto copied_constant = constant;
    auto copied_reference = reference;
    auto decayed_array = array;
    auto decayed_function = sample_function;
    auto& preserved_constant = constant;

    static_assert(std::is_same_v<decltype(copied_constant), int>);
    static_assert(std::is_same_v<decltype(copied_reference), int>);
    static_assert(std::is_same_v<decltype(decayed_array), int*>);
    static_assert(std::is_same_v<decltype(decayed_function), int (*)(double)>);
    static_assert(std::is_same_v<decltype(preserved_constant), const int&>);

    learn::ExampleChecks checks{"part2/stage02/section06/auto_deduces_decays"};
    LEARN_EXPECT_EQ(checks, decayed_array[1], 2);
    LEARN_EXPECT_EQ(checks, decayed_function(3.5), 3);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/auto_deduces_decays", run>;

}  // namespace
