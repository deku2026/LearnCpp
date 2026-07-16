// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section05_lifetime_details
// Item     : temporary_object_lifetime
// Topic id : part2/stage05/section05/temporary_object_lifetime
// References: [class.temporary]

#include "learn/example_support.hpp"

#include <string>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section05/temporary_object_lifetime";

class FullExpressionProbe {
public:
    explicit FullExpressionProbe(int value) : value_(value) { ++alive_; }
    ~FullExpressionProbe() { --alive_; }
    [[nodiscard]] int value() const noexcept { return value_; }
    [[nodiscard]] static int alive() noexcept { return alive_; }

private:
    int value_;
    inline static int alive_{};
};

int observe(const FullExpressionProbe& probe, int& alive_during_call) {
    alive_during_call = FullExpressionProbe::alive();
    return probe.value();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    int alive_during_call = 0;
    const int result = observe(FullExpressionProbe{21}, alive_during_call);
    LEARN_EXPECT_EQ(checks, result, 21);
    LEARN_EXPECT_EQ(checks, alive_during_call, 1);
    LEARN_EXPECT_EQ(checks, FullExpressionProbe::alive(), 0);  // Destroyed after full-expression.

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section05/temporary_object_lifetime", run>;

}  // namespace
