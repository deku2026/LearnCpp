// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : extern_c_link
// Topic id : part6/g/section01/extern_c_link
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <cstdint>
#include <string_view>
#include <type_traits>

extern "C" {
using LearnCppCAddFunction = std::int32_t(std::int32_t, std::int32_t) noexcept;

std::int32_t learn_cpp_c_add(std::int32_t left, std::int32_t right) noexcept {
    return left + right;
}
}

using CAddCallback = LearnCppCAddFunction*;
static_assert(std::is_same_v<decltype(&learn_cpp_c_add), CAddCallback>);

namespace {

constexpr std::string_view kTopic = "part6/g/section01/extern_c_link";

struct CApiV1 {
    std::uint32_t struct_size{};
    std::uint32_t version{};
    CAddCallback add{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const CApiV1 api{sizeof(CApiV1), 1, &learn_cpp_c_add};
    LEARN_EXPECT_EQ(checks, api.add(19, 23), 42);
    LEARN_EXPECT_EQ(checks, api.version, 1U);

#if 0
    extern "C" double learn_cpp_c_add(double, double);  // C linkage cannot be overloaded.
#endif
    // C language linkage makes a predictable boundary, but exact C ABI and
    // layout still require a documented platform/toolchain contract.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section01/extern_c_link", run>;

}  // namespace
