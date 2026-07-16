// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section06_class_misc
// Item     : const_and_mutable_member
// Topic id : part2/stage04/section06/const_and_mutable_member
// References: [dcl.stc], [class.mfct.non.static]

#include "learn/example_support.hpp"

#include <optional>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section06/const_and_mutable_member";

class SquaredValue {
public:
    explicit SquaredValue(int value) : value_(value) {}

    [[nodiscard]] int squared() const {
        if (!cache_) {
            cache_ = value_ * value_;
            ++computations_;
        }
        return *cache_;
    }

    void set(int value) {
        value_ = value;
        cache_.reset();
    }

    [[nodiscard]] int computations() const noexcept { return computations_; }

private:
    int value_;
    mutable std::optional<int> cache_;
    mutable int computations_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const SquaredValue fixed{4};
    LEARN_EXPECT_EQ(checks, fixed.squared(), 16);
    LEARN_EXPECT_EQ(checks, fixed.squared(), 16);
    LEARN_EXPECT_EQ(checks, fixed.computations(), 1);

    SquaredValue changing{3};
    LEARN_EXPECT_EQ(checks, changing.squared(), 9);
    changing.set(5);
    LEARN_EXPECT_EQ(checks, changing.squared(), 25);
    LEARN_EXPECT_EQ(checks, changing.computations(), 2);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section06/const_and_mutable_member", run>;

}  // namespace
