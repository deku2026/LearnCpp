// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : parameterized_and_fixture
// Topic id : part4/section04/parameterized_and_fixture

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <optional>

namespace {

[[nodiscard]] constexpr std::optional<int> factorial(int value) noexcept {
    if (value < 0 || value > 12) {
        return std::nullopt;
    }
    int result = 1;
    for (int factor = 2; factor <= value; ++factor) {
        result *= factor;
    }
    return result;
}

struct FixtureCounters {
    int set_up{};
    int tear_down{};
};

class CalculatorFixture {
public:
    explicit CalculatorFixture(FixtureCounters& counters) : counters_(counters) { ++counters_.set_up; }
    ~CalculatorFixture() { ++counters_.tear_down; }

    CalculatorFixture(const CalculatorFixture&) = delete;
    CalculatorFixture& operator=(const CalculatorFixture&) = delete;

    [[nodiscard]] int with_bias(int value) const noexcept { return value + bias_; }
    void set_bias(int value) noexcept { bias_ = value; }

private:
    FixtureCounters& counters_;
    int bias_{10};
};

struct FactorialCase {
    int input;
    std::optional<int> expected;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section04/parameterized_and_fixture"};

    constexpr std::array cases{FactorialCase{0, 1},
                               FactorialCase{1, 1},
                               FactorialCase{5, 120},
                               FactorialCase{12, 479001600},
                               FactorialCase{-1, std::nullopt},
                               FactorialCase{13, std::nullopt}};
    for (const auto& test : cases) {
        LEARN_EXPECT_EQ(checks, factorial(test.input), test.expected);
    }

    FixtureCounters counters;
    for (int parameter : {1, 2, 3}) {
        CalculatorFixture fixture{counters};  // A fresh fixture for every parameter.
        LEARN_EXPECT_EQ(checks, fixture.with_bias(parameter), parameter + 10);
        fixture.set_bias(100);  // This mutation cannot leak into the next case.
    }
    LEARN_EXPECT_EQ(checks, counters.set_up, 3);
    LEARN_EXPECT_EQ(checks, counters.tear_down, 3);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/parameterized_and_fixture", run>;

}  // namespace
