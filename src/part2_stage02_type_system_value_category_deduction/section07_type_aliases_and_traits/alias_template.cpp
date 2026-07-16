// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section07_type_aliases_and_traits
// Item     : alias_template
// Topic id : part2/stage02/section07/alias_template
//

#include "learn/example_support.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

template <class T>
using Sequence = std::vector<T>;

template <class T>
using SameTypePair = std::pair<T, T>;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_same_v<Sequence<int>, std::vector<int>>);
    static_assert(std::is_same_v<SameTypePair<long>, std::pair<long, long>>);

    Sequence<int> values{1, 2, 3};
    SameTypePair<int> endpoints{values.front(), values.back()};

    learn::ExampleChecks checks{"part2/stage02/section07/alias_template"};
    LEARN_EXPECT_EQ(checks, values.size(), std::size_t{3});
    LEARN_EXPECT_EQ(checks, endpoints.first, 1);
    LEARN_EXPECT_EQ(checks, endpoints.second, 3);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section07/alias_template", run>;

}  // namespace
