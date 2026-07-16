// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : coverage_overview
// Topic id : part4/section05/coverage_overview

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <cstdint>
#include <string_view>

namespace {

enum class Bucket : std::uint8_t { negative, zero, small, large };

[[nodiscard]] constexpr Bucket classify(int value) noexcept {
    if (value < 0) {
        return Bucket::negative;
    }
    if (value == 0) {
        return Bucket::zero;
    }
    return value < 10 ? Bucket::small : Bucket::large;
}

[[nodiscard]] constexpr bool eligible(int age, bool active, bool consent) noexcept {
    return age >= 18 && active && consent;
}

struct ClassificationCase {
    int input;
    Bucket expected;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section05/coverage_overview"};

    constexpr std::array cases{
        ClassificationCase{-1, Bucket::negative}, ClassificationCase{0, Bucket::zero},
        ClassificationCase{1, Bucket::small},     ClassificationCase{9, Bucket::small},
        ClassificationCase{10, Bucket::large},
    };
    std::uint8_t visited = 0;
    for (const auto& test : cases) {
        const auto actual = classify(test.input);
        LEARN_EXPECT_EQ(checks, actual, test.expected);
        visited |= static_cast<std::uint8_t>(1U << static_cast<unsigned int>(actual));
    }
    LEARN_EXPECT_EQ(checks, visited, 0x0FU);  // Every semantic branch was observed.

    // Baseline plus one independent change per condition gives MC/DC evidence.
    LEARN_EXPECT(checks, eligible(18, true, true));
    LEARN_EXPECT(checks, !eligible(17, true, true));
    LEARN_EXPECT(checks, !eligible(18, false, true));
    LEARN_EXPECT(checks, !eligible(18, true, false));

    constexpr std::string_view clang_flow =
        "-fprofile-instr-generate -fcoverage-mapping; llvm-profdata merge; llvm-cov show";
    constexpr std::string_view gcc_flow = "--coverage; gcov or lcov";
    LEARN_EXPECT(checks, clang_flow.contains("llvm-cov"));
    LEARN_EXPECT(checks, gcc_flow.contains("gcov"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/coverage_overview", run>;

}  // namespace
