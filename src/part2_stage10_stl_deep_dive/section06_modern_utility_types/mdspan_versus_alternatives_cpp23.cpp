// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : mdspan_versus_alternatives_cpp23
// Topic id : part2/stage10/section06/mdspan_versus_alternatives_cpp23

#ifdef __has_include
#if __has_include(<mdspan>)
#include <mdspan>
#define LEARN_HAS_MDSPAN_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <stdexcept>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/mdspan_versus_alternatives_cpp23";

class MatrixViewModel {
public:
    constexpr MatrixViewModel(int* data, std::size_t rows, std::size_t columns) noexcept
        : data_(data), rows_(rows), columns_(columns) {}

    [[nodiscard]] constexpr int& operator()(std::size_t row, std::size_t column) const noexcept {
        return data_[row * columns_ + column];
    }

    [[nodiscard]] int& at(std::size_t row, std::size_t column) const {
        if (row >= rows_ || column >= columns_) {
            throw std::out_of_range{"matrix index"};
        }
        return (*this)(row, column);
    }

    [[nodiscard]] constexpr std::size_t extent(std::size_t dimension) const noexcept {
        return dimension == 0 ? rows_ : columns_;
    }

private:
    int* data_;
    std::size_t rows_;
    std::size_t columns_;
};

template <class Matrix>
[[nodiscard]] int diagonal_sum(const Matrix& matrix) {
    const auto diagonal = std::min(matrix.extent(0), matrix.extent(1));
    int total{};
    for (std::size_t index = 0; index < diagonal; ++index) {
        total += matrix(index, index);
    }
    return total;
}

#if defined(LEARN_HAS_MDSPAN_HEADER) && defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
template <class Element, class Extents, class Layout, class Accessor>
[[nodiscard]] int mdspan_diagonal_sum(const std::mdspan<Element, Extents, Layout, Accessor>& matrix) {
    const auto diagonal = std::min(matrix.extent(0), matrix.extent(1));
    int total{};
    for (std::size_t index = 0; index < diagonal; ++index) {
        total += matrix[index, index];
    }
    return total;
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    std::array storage{1, 2, 3, 4, 5, 6};
    const MatrixViewModel model{storage.data(), 2, 3};
    LEARN_EXPECT_EQ(checks, model(1, 2), 6);
    LEARN_EXPECT_EQ(checks, diagonal_sum(model), 6);  // 1 + 5
    LEARN_EXPECT_THROWS(checks, std::out_of_range, model.at(2, 0));

    // Alternatives include manual index arithmetic, nested arrays (fixed shape), and a custom
    // view like the model above. mdspan standardizes rank, extents, mapping, and accessor policies.
#if defined(LEARN_HAS_MDSPAN_HEADER) && defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    const std::mdspan<int, std::dextents<std::size_t, 2>> matrix{storage.data(), 2, 3};
    LEARN_EXPECT_EQ(checks, (matrix[1, 2]), 6);
    LEARN_EXPECT_EQ(checks, mdspan_diagonal_sum(matrix), 6);
    matrix[0, 1] = 20;
    LEARN_EXPECT_EQ(checks, storage[1], 20);  // Neither view owns or copies the elements.
    LEARN_EXPECT_EQ(checks, matrix.size(), 6U);
    LEARN_EXPECT(checks, !matrix.empty());
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_mdspan >= 202207L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/mdspan_versus_alternatives_cpp23", run>;

}  // namespace
