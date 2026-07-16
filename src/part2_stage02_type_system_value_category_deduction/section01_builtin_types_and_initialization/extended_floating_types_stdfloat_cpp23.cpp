// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section01_builtin_types_and_initialization
// Item     : extended_floating_types_stdfloat_cpp23
// Topic id : part2/stage02/section01/extended_floating_types_stdfloat_cpp23
//

#include "learn/example_support.hpp"

#include <limits>
#include <type_traits>

#if defined(__has_include)
#if __has_include(<stdfloat>)
#include <stdfloat>
#define LEARNCPP_HAS_STDFLOAT_HEADER 1
#endif
#endif

#ifndef LEARNCPP_HAS_STDFLOAT_HEADER
#define LEARNCPP_HAS_STDFLOAT_HEADER 0
#endif

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if LEARNCPP_HAS_STDFLOAT_HEADER && defined(__STDCPP_FLOAT32_T__)
    constexpr std::float32_t value = 1.5F32;
    static_assert(std::is_floating_point_v<std::float32_t>);
    static_assert(std::numeric_limits<std::float32_t>::is_iec559);

    learn::ExampleChecks checks{"part2/stage02/section01/extended_floating_types_stdfloat_cpp23"};
    LEARN_EXPECT(checks, value > std::float32_t{1});
    LEARN_EXPECT(checks, value < std::float32_t{2});
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage02/section01/extended_floating_types_stdfloat_cpp23",
                                             "<stdfloat> with __STDCPP_FLOAT32_T__");
#endif
}

#undef LEARNCPP_HAS_STDFLOAT_HEADER

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/extended_floating_types_stdfloat_cpp23", run>;

}  // namespace
