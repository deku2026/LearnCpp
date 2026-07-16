// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section10_library_protocol_patches_cpp23
// Item     : tuple_like_compat_cpp23
// Topic id : part6/e/section10/tuple_like_compat_cpp23

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <concepts>
#include <tuple>
#include <type_traits>
#include <utility>
#include <version>

namespace {

template <class TupleLike>
[[nodiscard]] constexpr auto sum_tuple_like(const TupleLike& values) {
    return std::apply([](const auto&... items) { return (items + ... + 0); }, values);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section10/tuple_like_compat_cpp23"};

    constexpr std::array values{10, 20, 12};
    static_assert(sum_tuple_like(values) == 42);
    const auto [first, second, third] = values;
    LEARN_EXPECT_EQ(checks, first + second + third, 42);

#if defined(__cpp_lib_tuple_like) && __cpp_lib_tuple_like >= 202207L
    const std::tuple<long, long, long> converted{values};
    LEARN_EXPECT_EQ(checks, std::get<0>(converted), 10L);
    LEARN_EXPECT_EQ(checks, sum_tuple_like(converted), 42L);
#else
    const auto converted = std::apply([](auto... items) { return std::tuple<long, long, long>{items...}; }, values);
    LEARN_EXPECT_EQ(checks, sum_tuple_like(converted), 42L);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section10/tuple_like_compat_cpp23", run>;

}  // namespace
