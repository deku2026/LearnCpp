// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section02_compile_and_link_model
// Item     : inline_template_versus_odr
// Topic id : part6/g/section02/inline_template_versus_odr
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <concepts>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/g/section02/inline_template_versus_odr";

inline constexpr int default_limit = 100;

[[nodiscard]] inline constexpr int clamp_to_default(int value) {
    return value > default_limit ? default_limit : value;
}

template <class T>
    requires(std::integral<T> || std::floating_point<T>)
[[nodiscard]] constexpr T square(T value) {
    return value * value;
}

static_assert(square(6) == 36);
static_assert(std::is_same_v<decltype(square(2.0)), double>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT_EQ(checks, clamp_to_default(150), 100);
    LEARN_EXPECT_EQ(checks, square(7), 49);
    LEARN_EXPECT_EQ(checks, square(1.5), 2.25);

    // inline permits equivalent definitions in multiple TUs; templates need
    // their definition visible for instantiation. Neither relaxes the demand
    // that corresponding definitions be token-equivalent under the ODR.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section02/inline_template_versus_odr", run>;

}  // namespace
