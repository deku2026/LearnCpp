// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section05_crtp_and_policy_based_design
// Item     : crtp_deducing_this_simplification_cpp23
// Topic id : part6/e/section05/crtp_deducing_this_simplification_cpp23

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <type_traits>
#include <utility>

namespace {

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
#define LEARNCPP_HAS_EXPLICIT_THIS 1
class Counter {
public:
    template <class Self>
    constexpr void add(this Self& self, int amount) noexcept {
        self.value_ += amount;
    }

    template <class Self>
    [[nodiscard]] constexpr decltype(auto) value(this Self&& self) noexcept {
        return (std::forward<Self>(self).value_);
    }

private:
    int value_{};
};
#else
#define LEARNCPP_HAS_EXPLICIT_THIS 0
template <class Derived>
class CounterOperations {
public:
    constexpr void add(int amount) noexcept { static_cast<Derived&>(*this).stored_value() += amount; }
    [[nodiscard]] constexpr int value() const noexcept { return static_cast<const Derived&>(*this).stored_value(); }
};

class Counter : public CounterOperations<Counter> {
public:
    [[nodiscard]] constexpr int& stored_value() noexcept { return value_; }
    [[nodiscard]] constexpr const int& stored_value() const noexcept { return value_; }

private:
    int value_{};
};
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section05/crtp_deducing_this_simplification_cpp23"};

    Counter counter;
    counter.add(40);
    counter.add(2);
    LEARN_EXPECT_EQ(checks, counter.value(), 42);
#if LEARNCPP_HAS_EXPLICIT_THIS
    static_assert(std::is_same_v<decltype(std::as_const(counter).value()), const int&>);
#else
    static_assert(LEARNCPP_HAS_EXPLICIT_THIS == 0);
#endif

    return checks.result();
}

#undef LEARNCPP_HAS_EXPLICIT_THIS

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section05/crtp_deducing_this_simplification_cpp23", run>;

}  // namespace
