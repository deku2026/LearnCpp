// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : default_initialization
// Topic id : part2/stage02/section02/default_initialization
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

struct SafeDefault {
    int value = 17;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_default_constructible_v<SafeDefault>);

    int fundamental;  // Default-initialized: its value is indeterminate.
    int* address = &fundamental;
    fundamental = 23;  // Assign before the first read.
    SafeDefault object;

    learn::ExampleChecks checks{"part2/stage02/section02/default_initialization"};
    LEARN_EXPECT(checks, address == &fundamental);
    LEARN_EXPECT_EQ(checks, fundamental, 23);
    LEARN_EXPECT_EQ(checks, object.value, 17);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/default_initialization", run>;

}  // namespace
