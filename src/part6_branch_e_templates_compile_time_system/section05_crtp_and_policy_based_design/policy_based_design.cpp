// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section05_crtp_and_policy_based_design
// Item     : policy_based_design
// Topic id : part6/e/section05/policy_based_design

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <algorithm>
#include <concepts>
#include <type_traits>

namespace {

struct UncheckedPolicy {
    [[nodiscard]] static constexpr int normalize(int value) noexcept { return value; }
};

struct ClampToBytePolicy {
    [[nodiscard]] static constexpr int normalize(int value) noexcept { return std::clamp(value, 0, 255); }
};

template <class Policy>
concept ValuePolicy = requires(int value) {
    { Policy::normalize(value) } noexcept -> std::same_as<int>;
};

template <ValuePolicy Policy>
class BasicValue : private Policy {
public:
    explicit constexpr BasicValue(int value) noexcept : value_(Policy::normalize(value)) {}
    constexpr void set(int value) noexcept { value_ = Policy::normalize(value); }
    [[nodiscard]] constexpr int get() const noexcept { return value_; }

private:
    int value_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section05/policy_based_design"};

    static_assert(BasicValue<UncheckedPolicy>{300}.get() == 300);
    static_assert(BasicValue<ClampToBytePolicy>{300}.get() == 255);
    static_assert(sizeof(BasicValue<ClampToBytePolicy>) == sizeof(int));
    BasicValue<ClampToBytePolicy> value{-10};
    LEARN_EXPECT_EQ(checks, value.get(), 0);
    value.set(42);
    LEARN_EXPECT_EQ(checks, value.get(), 42);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section05/policy_based_design", run>;

}  // namespace
