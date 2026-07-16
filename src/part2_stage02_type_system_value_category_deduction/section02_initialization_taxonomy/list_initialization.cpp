// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : list_initialization
// Topic id : part2/stage02/section02/list_initialization
//

#include "learn/example_support.hpp"

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    int direct_list{5};
    int copy_list = {6};
    std::vector<int> elements{2, 9};
    std::vector<int> empty{};
    static_assert(std::is_same_v<decltype(direct_list), int>);
    static_assert(std::is_same_v<decltype(copy_list), int>);

    learn::ExampleChecks checks{"part2/stage02/section02/list_initialization"};
    LEARN_EXPECT_EQ(checks, direct_list, 5);
    LEARN_EXPECT_EQ(checks, copy_list, 6);
    LEARN_EXPECT_EQ(checks, elements.size(), std::size_t{2});
    LEARN_EXPECT(checks, empty.empty());

    // int rejected{2.5}; would be ill-formed because list initialization narrows.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/list_initialization", run>;

}  // namespace
