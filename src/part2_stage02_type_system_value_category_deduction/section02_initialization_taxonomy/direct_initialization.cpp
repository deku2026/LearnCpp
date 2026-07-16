// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : direct_initialization
// Topic id : part2/stage02/section02/direct_initialization
//

#include "learn/example_support.hpp"

#include <string>
#include <type_traits>

namespace {

struct ExplicitCount {
    explicit ExplicitCount(int input) : value(input) {}
    int value;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_constructible_v<ExplicitCount, int>);
    static_assert(!std::is_convertible_v<int, ExplicitCount>);

    ExplicitCount count(4);
    std::string repeated(3, 'x');
    const double source = 3.75;
    const int narrowed(source);  // Direct non-list initialization permits narrowing.

    learn::ExampleChecks checks{"part2/stage02/section02/direct_initialization"};
    LEARN_EXPECT_EQ(checks, count.value, 4);
    LEARN_EXPECT_EQ(checks, repeated, std::string{"xxx"});
    LEARN_EXPECT_EQ(checks, narrowed, 3);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/direct_initialization", run>;

}  // namespace
