// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : multidim_operator_subscript_cpp23
// Topic id : part2/stage04/section05/multidim_operator_subscript_cpp23
// References: [over.sub], P2128R6, SD-FeatureTest __cpp_multidimensional_subscript

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section05/multidim_operator_subscript_cpp23";

class Matrix2x2 {
public:
#if defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202211L
    int& operator[](std::size_t row, std::size_t column) noexcept { return data_[row * 2 + column]; }
    const int& operator[](std::size_t row, std::size_t column) const noexcept { return data_[row * 2 + column]; }
#else
    int& at(std::size_t row, std::size_t column) noexcept { return data_[row * 2 + column]; }
    const int& at(std::size_t row, std::size_t column) const noexcept { return data_[row * 2 + column]; }
#endif

private:
    std::array<int, 4> data_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Matrix2x2 matrix;
#if defined(__cpp_multidimensional_subscript) && __cpp_multidimensional_subscript >= 202211L
    matrix[0, 1] = 7;
    matrix[1, 0] = 9;
    LEARN_EXPECT_EQ(checks, (matrix[0, 1]), 7);
    LEARN_EXPECT_EQ(checks, (matrix[1, 0]), 9);
#else
    matrix.at(0, 1) = 7;
    matrix.at(1, 0) = 9;
    LEARN_EXPECT_EQ(checks, matrix.at(0, 1), 7);
    LEARN_EXPECT_EQ(checks, matrix.at(1, 0), 9);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage04/section05/multidim_operator_subscript_cpp23", run>;

}  // namespace
