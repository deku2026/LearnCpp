// Runnable teaching example
// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : calling_conventions
// Topic id : part6/g/section01/calling_conventions
// References: C++23 [basic.link], [basic.def.odr]; platform ABI/linker documentation

#include "learn/example_support.hpp"

#include <cstdint>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/g/section01/calling_conventions";

using BinaryOperation = int (*)(int, int) noexcept;

[[nodiscard]] int add_values(int left, int right) noexcept {
    return left + right;
}

struct FunctionTable {
    std::uint32_t size{};
    std::uint32_t abi_version{};
    BinaryOperation operation{};
};

static_assert(std::is_pointer_v<BinaryOperation>);
static_assert(std::is_trivially_copyable_v<FunctionTable>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const FunctionTable api{sizeof(FunctionTable), 1, &add_values};
    LEARN_EXPECT_EQ(checks, api.size, static_cast<std::uint32_t>(sizeof(FunctionTable)));
    LEARN_EXPECT_EQ(checks, api.abi_version, 1U);
    LEARN_EXPECT_EQ(checks, api.operation(20, 22), 42);

#if defined(_M_X64) || defined(__x86_64__)
    constexpr std::string_view target_note = "64-bit target: consult the platform x64 ABI";
#elif defined(_M_IX86) || defined(__i386__)
    constexpr std::string_view target_note = "32-bit x86: calling-convention choice is observable";
#else
    constexpr std::string_view target_note = "consult this target's processor ABI";
#endif
    LEARN_EXPECT(checks, !target_note.empty());

    // C++ specifies compatible function types, not register allocation, stack
    // cleanup, or binary calling conventions. Those belong to the target ABI.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/g/section01/calling_conventions", run>;

}  // namespace
