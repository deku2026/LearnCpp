// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : uninitialized_read_is_ub
// Topic id : part2/stage02/section02/uninitialized_read_is_ub
//

#include "learn/example_support.hpp"

#include <optional>
#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_trivially_default_constructible_v<int>);

    int raw;
    int* raw_address = &raw;  // Taking the address does not read the value.
    raw = 31;

    std::optional<int> guarded;
    learn::ExampleChecks checks{"part2/stage02/section02/uninitialized_read_is_ub"};
    LEARN_EXPECT(checks, raw_address == &raw);
    LEARN_EXPECT_EQ(checks, raw, 31);
    LEARN_EXPECT(checks, !guarded.has_value());
    guarded = 32;
    LEARN_EXPECT_EQ(checks, guarded.value(), 32);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/uninitialized_read_is_ub", run>;

}  // namespace
