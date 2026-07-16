// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section05_optimization_safety_valves
// Item     : ftrapv
// Topic id : part6/g/section05/ftrapv

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <limits>
#include <optional>
#include <string_view>

namespace {

[[nodiscard]] constexpr std::optional<int> checked_add(int left, int right) noexcept {
    if ((right > 0 && left > std::numeric_limits<int>::max() - right) ||
        (right < 0 && left < std::numeric_limits<int>::min() - right)) {
        return std::nullopt;
    }
    return left + right;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section05/ftrapv"};

    LEARN_EXPECT_EQ(checks, checked_add(20, 22), std::optional<int>{42});
    LEARN_EXPECT(checks, !checked_add(std::numeric_limits<int>::max(), 1).has_value());
    LEARN_EXPECT(checks, !checked_add(std::numeric_limits<int>::min(), -1).has_value());

    constexpr std::string_view flag{"-ftrapv"};
    constexpr std::string_view limitation =
        "diagnostic safety net, not a replacement for an explicit arithmetic contract";
    LEARN_EXPECT(checks, flag.ends_with("trapv"));
    LEARN_EXPECT(checks, limitation.contains("not a replacement"));

#if 0
    int overflow = std::numeric_limits<int>::max() + 1;
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section05/ftrapv", run>;

}  // namespace
