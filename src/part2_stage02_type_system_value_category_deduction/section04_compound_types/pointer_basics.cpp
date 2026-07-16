// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section04_compound_types
// Item     : pointer_basics
// Topic id : part2/stage02/section04/pointer_basics
//

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::array<int, 3> values{10, 20, 30};
    int* first = values.data();
    int* second = first + 1;
    int* one_past = first + values.size();
    int* null_pointer = nullptr;

    static_assert(std::is_pointer_v<decltype(first)>);
    learn::ExampleChecks checks{"part2/stage02/section04/pointer_basics"};
    LEARN_EXPECT_EQ(checks, *first, 10);
    LEARN_EXPECT_EQ(checks, *second, 20);
    LEARN_EXPECT_EQ(checks, one_past - first, std::ptrdiff_t{3});
    LEARN_EXPECT(checks, null_pointer == nullptr);

    // Forming one-past is valid; dereferencing one_past would be undefined.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section04/pointer_basics", run>;

}  // namespace
