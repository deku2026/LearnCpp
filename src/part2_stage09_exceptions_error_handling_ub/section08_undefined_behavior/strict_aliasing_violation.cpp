// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : strict_aliasing_violation
// Topic id : part2/stage09/section08/strict_aliasing_violation
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <bit>
#include <cstdint>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section08/strict_aliasing_violation";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr float one = 1.0F;
    constexpr std::uint32_t bits = std::bit_cast<std::uint32_t>(one);
    static_assert(sizeof(float) == sizeof(std::uint32_t));
    LEARN_EXPECT_EQ(checks, std::bit_cast<float>(bits), one);
#if 0
    const std::uint32_t undefined = *reinterpret_cast<const std::uint32_t*>(&one);
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section08/strict_aliasing_violation", run>;

}  // namespace
