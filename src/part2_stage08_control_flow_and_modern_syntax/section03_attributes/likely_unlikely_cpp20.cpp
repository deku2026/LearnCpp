// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : likely_unlikely_cpp20
// Topic id : part2/stage08/section03/likely_unlikely_cpp20
//

#include "learn/example_support.hpp"

namespace {

enum class Classification { ordinary, exceptional };

[[nodiscard]] constexpr Classification classify(const int value) noexcept {
    if (value != 0) [[likely]] {
        return Classification::ordinary;
    }
    [[unlikely]] return Classification::exceptional;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section03/likely_unlikely_cpp20"};
    LEARN_EXPECT_EQ(checks, classify(7), Classification::ordinary);
    LEARN_EXPECT_EQ(checks, classify(-7), Classification::ordinary);
    LEARN_EXPECT_EQ(checks, classify(0), Classification::exceptional);

    // These attributes express optimization likelihood only; they never change the result.
    static_assert(classify(0) == Classification::exceptional);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/likely_unlikely_cpp20", run>;

}  // namespace
