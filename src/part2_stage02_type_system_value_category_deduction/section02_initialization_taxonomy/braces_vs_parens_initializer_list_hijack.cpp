// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : braces_vs_parens_initializer_list_hijack
// Topic id : part2/stage02/section02/braces_vs_parens_initializer_list_hijack
//

#include "learn/example_support.hpp"

#include <cstddef>
#include <type_traits>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::vector<int> repeated(3, 7);
    std::vector<int> two_elements{3, 7};
    static_assert(std::is_constructible_v<std::vector<int>, std::size_t, int>);

    learn::ExampleChecks checks{"part2/stage02/section02/braces_vs_parens_initializer_list_hijack"};
    LEARN_EXPECT_EQ(checks, repeated.size(), std::size_t{3});
    LEARN_EXPECT_EQ(checks, repeated[1], 7);
    LEARN_EXPECT_EQ(checks, two_elements.size(), std::size_t{2});
    LEARN_EXPECT_EQ(checks, two_elements[0], 3);
    return checks.result();
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage02/section02/braces_vs_parens_initializer_list_hijack", run>;

}  // namespace
