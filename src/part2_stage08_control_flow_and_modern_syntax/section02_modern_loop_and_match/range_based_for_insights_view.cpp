// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section02_modern_loop_and_match
// Item     : range_based_for_insights_view
// Topic id : part2/stage08/section02/range_based_for_insights_view
//

#include "learn/example_support.hpp"

#include <array>

namespace {

class InstrumentedRange {
public:
    InstrumentedRange(std::array<int, 3> values, int& begin_calls, int& end_calls) noexcept
        : values_(values), begin_calls_(&begin_calls), end_calls_(&end_calls) {}

    [[nodiscard]] auto begin() noexcept {
        ++*begin_calls_;
        return values_.begin();
    }

    [[nodiscard]] auto end() noexcept {
        ++*end_calls_;
        return values_.end();
    }

    [[nodiscard]] const std::array<int, 3>& values() const noexcept { return values_; }

private:
    std::array<int, 3> values_;
    int* begin_calls_;
    int* end_calls_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage08/section02/range_based_for_insights_view"};
    int begin_calls = 0;
    int end_calls = 0;
    InstrumentedRange range{{1, 2, 3}, begin_calls, end_calls};

    for (int& value : range) {
        value *= 10;
    }

    LEARN_EXPECT_EQ(checks, begin_calls, 1);
    LEARN_EXPECT_EQ(checks, end_calls, 1);
    LEARN_EXPECT_EQ(checks, range.values(), (std::array{10, 20, 30}));

    // A range-for behaves roughly as if it first bound `auto&& range = expression`,
    // then obtained begin/end once. Using a reference loop variable preserves mutation.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section02/range_based_for_insights_view", run>;

}  // namespace
