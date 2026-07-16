// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section05_lifetime_details
// Item     : const_ref_extends_temporary
// Topic id : part2/stage05/section05/const_ref_extends_temporary
// References: [class.temporary]

#include "learn/example_support.hpp"

#include <string>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section05/const_ref_extends_temporary";

class LifetimeProbe {
public:
    explicit LifetimeProbe(std::string value) : value_(std::move(value)) { ++alive_; }
    ~LifetimeProbe() { --alive_; }
    [[nodiscard]] const std::string& value() const noexcept { return value_; }
    [[nodiscard]] static int alive() noexcept { return alive_; }

private:
    std::string value_;
    inline static int alive_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT_EQ(checks, LifetimeProbe::alive(), 0);
    {
        const LifetimeProbe& reference = LifetimeProbe{"extended to reference scope"};
        LEARN_EXPECT_EQ(checks, LifetimeProbe::alive(), 1);
        LEARN_EXPECT_EQ(checks, reference.value(), std::string{"extended to reference scope"});
    }
    LEARN_EXPECT_EQ(checks, LifetimeProbe::alive(), 0);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section05/const_ref_extends_temporary", run>;

}  // namespace
