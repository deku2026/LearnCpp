// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : size_t_ptrdiff_literal_suffix_cpp23
// Topic id : part2/stage02/section01/size_t_ptrdiff_literal_suffix_cpp23
//

#include "learn/example_support.hpp"

#include <cstddef>
#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_size_t_suffix) && __cpp_size_t_suffix >= 202011L
    constexpr auto count = 42uz;
    constexpr auto offset = 42z;
    static_assert(std::is_same_v<decltype(count), const std::size_t>);
    static_assert(std::is_same_v<decltype(offset), const std::make_signed_t<std::size_t>>);

    learn::ExampleChecks checks{"part2/stage02/section01/size_t_ptrdiff_literal_suffix_cpp23"};
    LEARN_EXPECT_EQ(checks, count, std::size_t{42});
    LEARN_EXPECT_EQ(checks, offset, std::make_signed_t<std::size_t>{42});
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage02/section01/size_t_ptrdiff_literal_suffix_cpp23",
                                             "__cpp_size_t_suffix");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/size_t_ptrdiff_literal_suffix_cpp23", run>;

}  // namespace
