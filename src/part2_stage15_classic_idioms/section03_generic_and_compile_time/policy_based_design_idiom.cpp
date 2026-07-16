// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : policy_based_design_idiom
// Topic id : part2/stage15/section03/policy_based_design_idiom
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <algorithm>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section03/policy_based_design_idiom";

struct RejectOutsideRange {
    static int apply(int value, int low, int high) { return value >= low && value <= high ? value : low; }
};

struct ClampToRange {
    static int apply(int value, int low, int high) { return std::clamp(value, low, high); }
};

template <class RangePolicy>
class Setting {
public:
    Setting(int low, int high) : low_(low), high_(high), value_(low) {}
    void set(int value) { value_ = RangePolicy::apply(value, low_, high_); }
    int get() const noexcept { return value_; }

private:
    int low_;
    int high_;
    int value_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Setting<RejectOutsideRange> rejecting{0, 10};
    Setting<ClampToRange> clamping{0, 10};
    rejecting.set(20);
    clamping.set(20);
    LEARN_EXPECT_EQ(checks, rejecting.get(), 0);
    LEARN_EXPECT_EQ(checks, clamping.get(), 10);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section03/policy_based_design_idiom", run>;

}  // namespace
