// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : name_mangling_with_cppfilt
// Topic id : part6/g/section01/name_mangling_with_cppfilt
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <array>
#include <string_view>

namespace branch_g_mangling_demo {

[[nodiscard]] constexpr int compute(int value) {
    return value + 1;
}
[[nodiscard]] constexpr double compute(double value) {
    return value + 0.5;
}

template <class T>
[[nodiscard]] constexpr T identity(T value) {
    return value;
}

}  // namespace branch_g_mangling_demo

extern "C" int branch_g_plain_c_symbol(int value) noexcept {
    return value * 2;
}

namespace {

constexpr std::string_view kTopic = "part6/g/section01/name_mangling_with_cppfilt";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    int (*integer_overload)(int) = &branch_g_mangling_demo::compute;
    double (*floating_overload)(double) = &branch_g_mangling_demo::compute;
    LEARN_EXPECT_EQ(checks, integer_overload(4), 5);
    LEARN_EXPECT_EQ(checks, floating_overload(4.0), 4.5);
    LEARN_EXPECT_EQ(checks, branch_g_mangling_demo::identity(9), 9);
    LEARN_EXPECT_EQ(checks, branch_g_plain_c_symbol(6), 12);

    constexpr std::array inspection_tools{"nm", "c++filt", "dumpbin /symbols"};
    LEARN_EXPECT_EQ(checks, inspection_tools.size(), 3U);
    // Mangling lets binary symbols distinguish the overloads and template
    // specialization. Exact spelling is ABI-specific; never parse it in code.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section01/name_mangling_with_cppfilt", run>;

}  // namespace
