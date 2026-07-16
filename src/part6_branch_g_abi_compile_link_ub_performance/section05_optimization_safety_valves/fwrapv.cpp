// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section05_optimization_safety_valves
// Item     : fwrapv
// Topic id : part6/g/section05/fwrapv

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstdint>
#include <limits>
#include <optional>
#include <string_view>

namespace {

[[nodiscard]] constexpr std::optional<int> portable_increment(int value) noexcept {
    if (value == std::numeric_limits<int>::max()) {
        return std::nullopt;
    }
    return value + 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section05/fwrapv"};

    LEARN_EXPECT_EQ(checks, portable_increment(41), std::optional<int>{42});
    LEARN_EXPECT(checks, !portable_increment(std::numeric_limits<int>::max()).has_value());

    constexpr std::uint32_t maximum = std::numeric_limits<std::uint32_t>::max();
    constexpr std::uint32_t wrapped = maximum + 1U;
    static_assert(wrapped == 0U);  // Portable source uses an unsigned modular domain.
    LEARN_EXPECT_EQ(checks, wrapped, 0U);

    constexpr std::string_view flag{"-fwrapv"};
    constexpr std::string_view scope =
        "GCC/Clang option defining signed add, subtract, and multiply overflow as wrapping";
    LEARN_EXPECT(checks, flag.ends_with("wrapv"));
    LEARN_EXPECT(checks, scope.contains("option"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section05/fwrapv", run>;

}  // namespace
