// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : signed_overflow_deep
// Topic id : part6/g/section03/signed_overflow_deep

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstdint>
#include <limits>
#include <optional>

namespace {

[[nodiscard]] constexpr std::optional<int> checked_add(int left, int right) noexcept {
    constexpr int maximum = std::numeric_limits<int>::max();
    constexpr int minimum = std::numeric_limits<int>::min();
    if ((right > 0 && left > maximum - right) || (right < 0 && left < minimum - right)) {
        return std::nullopt;
    }
    return left + right;
}

[[nodiscard]] constexpr std::optional<int> checked_multiply(int left, int right) noexcept {
    const auto wide = static_cast<std::int64_t>(left) * static_cast<std::int64_t>(right);
    if (wide > std::numeric_limits<int>::max() || wide < std::numeric_limits<int>::min()) {
        return std::nullopt;
    }
    return static_cast<int>(wide);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section03/signed_overflow_deep"};

    static_assert(checked_add(40, 2) == 42);
    static_assert(checked_multiply(6, 7) == 42);
    LEARN_EXPECT(checks, !checked_add(std::numeric_limits<int>::max(), 1).has_value());
    LEARN_EXPECT(checks, !checked_multiply(std::numeric_limits<int>::max(), 2).has_value());

    constexpr std::uint32_t modular = std::numeric_limits<std::uint32_t>::max() + 1U;
    static_assert(modular == 0U);  // Unsigned arithmetic is defined modulo 2^N.
    LEARN_EXPECT_EQ(checks, modular, 0U);

#if 0
    int overflow = std::numeric_limits<int>::max() + 1;
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/signed_overflow_deep", run>;

}  // namespace
