// Doc      : 第2部分-阶段8-控制流与现代语法增强.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : range_for_temporary_extension_cpp23
// Topic id : part2/stage08/section04/range_for_temporary_extension_cpp23
//

#include "learn/example_support.hpp"

#include <array>

namespace {

#if defined(__cpp_range_based_for) && __cpp_range_based_for >= 202211L
class TrackedRange {
public:
    explicit TrackedRange(bool& alive) noexcept : alive_(&alive) { *alive_ = true; }

    TrackedRange(const TrackedRange&) = delete;
    TrackedRange& operator=(const TrackedRange&) = delete;
    TrackedRange(TrackedRange&&) = delete;
    TrackedRange& operator=(TrackedRange&&) = delete;

    ~TrackedRange() { *alive_ = false; }

    [[nodiscard]] const std::array<int, 3>& values() const noexcept { return values_; }

private:
    bool* alive_;
    std::array<int, 3> values_{2, 4, 6};
};
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_range_based_for) && __cpp_range_based_for >= 202211L
    learn::ExampleChecks checks{"part2/stage08/section04/range_for_temporary_extension_cpp23"};
    bool owner_alive = false;
    int sum = 0;

    for (const int value : TrackedRange{owner_alive}.values()) {
        LEARN_EXPECT(checks, owner_alive);
        sum += value;
    }

    LEARN_EXPECT_EQ(checks, sum, 12);
    LEARN_EXPECT(checks, !owner_alive);

#if 0
    // C++23 does not extend the lifetime of by-value function parameters destroyed
    // inside the callee, so returning a reference through such a helper still dangles.
    for (const int value : through_by_value_parameter(TrackedRange{owner_alive}).values()) {
        (void)value;
    }
#endif

    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage08/section04/range_for_temporary_extension_cpp23",
                                             "C++23 range-for temporary lifetime extension");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section04/range_for_temporary_extension_cpp23", run>;

}  // namespace
