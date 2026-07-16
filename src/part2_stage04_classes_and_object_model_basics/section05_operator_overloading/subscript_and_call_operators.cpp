// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : subscript_and_call_operators
// Topic id : part2/stage04/section05/subscript_and_call_operators
// References: [over.sub], [over.call]

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section05/subscript_and_call_operators";

class Polynomial {
public:
    explicit Polynomial(std::array<int, 3> coefficients) : coefficients_(coefficients) {}

    int& operator[](std::size_t index) { return coefficients_.at(index); }
    const int& operator[](std::size_t index) const { return coefficients_.at(index); }
    [[nodiscard]] int operator()(int x) const noexcept {
        return coefficients_[0] + coefficients_[1] * x + coefficients_[2] * x * x;
    }

private:
    std::array<int, 3> coefficients_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Polynomial polynomial{{1, 2, 3}};
    LEARN_EXPECT_EQ(checks, polynomial(2), 17);
    polynomial[0] = 5;
    LEARN_EXPECT_EQ(checks, polynomial(0), 5);
    LEARN_EXPECT_THROWS(checks, std::out_of_range, polynomial[3]);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section05/subscript_and_call_operators", run>;

}  // namespace
