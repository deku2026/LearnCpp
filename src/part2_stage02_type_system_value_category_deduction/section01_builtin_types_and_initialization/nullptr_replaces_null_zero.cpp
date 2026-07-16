// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : nullptr_replaces_null_zero
// Topic id : part2/stage02/section01/nullptr_replaces_null_zero
//

#include "learn/example_support.hpp"

#include <cstddef>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view selected_overload(int*) {
    return "pointer";
}

constexpr std::string_view selected_overload(int) {
    return "integer";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_same_v<decltype(nullptr), std::nullptr_t>);
    static_assert(!std::is_convertible_v<std::nullptr_t, int>);

    int* pointer = nullptr;
    learn::ExampleChecks checks{"part2/stage02/section01/nullptr_replaces_null_zero"};
    LEARN_EXPECT(checks, pointer == nullptr);
    LEARN_EXPECT_EQ(checks, selected_overload(nullptr), std::string_view{"pointer"});
    LEARN_EXPECT_EQ(checks, selected_overload(0), std::string_view{"integer"});

    // NULL may be an integer constant and can select or ambiguate the wrong overload.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/nullptr_replaces_null_zero", run>;

}  // namespace
