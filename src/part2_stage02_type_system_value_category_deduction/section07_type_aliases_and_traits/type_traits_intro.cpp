// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section07_type_aliases_and_traits
// Item     : type_traits_intro
// Topic id : part2/stage02/section07/type_traits_intro
//

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    using Complicated = const int&;
    using Plain = std::remove_cvref_t<Complicated>;

    static_assert(std::is_integral_v<Plain>);
    static_assert(std::is_same_v<Plain, int>);
    static_assert(std::is_constructible_v<std::string, std::size_t, char>);
    static_assert(!std::is_copy_constructible_v<std::unique_ptr<int>>);

    learn::ExampleChecks checks{"part2/stage02/section07/type_traits_intro"};
    LEARN_EXPECT(checks, std::is_trivially_copyable_v<int>);
    LEARN_EXPECT(checks, std::is_nothrow_move_constructible_v<std::string>);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section07/type_traits_intro", run>;

}  // namespace
