// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section07_type_aliases_and_traits
// Item     : using_alias_replaces_typedef
// Topic id : part2/stage02/section07/using_alias_replaces_typedef
//

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

typedef int (*LegacyCallback)(int);
using Callback = int (*)(int);

int increment(int value) {
    return value + 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_same_v<LegacyCallback, Callback>);
    Callback callback = &increment;

    learn::ExampleChecks checks{"part2/stage02/section07/using_alias_replaces_typedef"};
    LEARN_EXPECT_EQ(checks, callback(8), 9);

    // Unlike typedef, a using-declaration can directly define an alias template.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section07/using_alias_replaces_typedef", run>;

}  // namespace
