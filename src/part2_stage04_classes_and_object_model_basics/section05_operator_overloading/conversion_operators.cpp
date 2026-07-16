// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : conversion_operators
// Topic id : part2/stage04/section05/conversion_operators
// References: [class.conv.fct]

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section05/conversion_operators";

class Celsius {
public:
    explicit constexpr Celsius(double value) : value_(value) {}
    constexpr operator double() const noexcept { return value_; }

private:
    double value_;
};

constexpr double average(double left, double right) noexcept {
    return (left + right) / 2.0;
}

static_assert(std::is_convertible_v<Celsius, double>);
static_assert(!std::is_convertible_v<double, Celsius>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    constexpr Celsius morning{18.0};
    constexpr Celsius afternoon{24.0};
    static_assert(average(morning, afternoon) == 21.0);
    LEARN_EXPECT_EQ(checks, static_cast<double>(morning), 18.0);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section05/conversion_operators", run>;

}  // namespace
