// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section05_operator_overloading
// Item     : multidim_operator_subscript_cpp23
// Topic id : part2/stage04/section05/multidim_operator_subscript_cpp23
//
// Covers: C++23 multi-parameter operator[] for 2D indexing

#include "learn/topic_registry.hpp"

#include <array>
#include <cstddef>

namespace {

#if defined(__cpp_multidimensional_subscript) || (defined(__cplusplus) && __cplusplus >= 202302L)

template <class T, std::size_t Rows, std::size_t Cols>
class Matrix {
    std::array<T, Rows * Cols> data_{};

public:
    T& operator[](std::size_t r, std::size_t c) { return data_[r * Cols + c]; }
    const T& operator[](std::size_t r, std::size_t c) const { return data_[r * Cols + c]; }
    static constexpr std::size_t rows() { return Rows; }
    static constexpr std::size_t cols() { return Cols; }
};

void demo_basics() {
    Matrix<int, 2, 3> m;
    m[0, 0] = 1;
    m[1, 2] = 9;
    LEARN_CHECK((m[0, 0] == 1));
    LEARN_CHECK((m[1, 2] == 9));
}

void demo_intermediate() {
    Matrix<int, 2, 3> m;
    for (std::size_t i = 0; i < m.rows(); ++i) {
        for (std::size_t j = 0; j < m.cols(); ++j) {
            m[i, j] = static_cast<int>(i * m.cols() + j);
        }
    }
    LEARN_CHECK((m[0, 1] == 1));
    LEARN_CHECK((m[1, 0] == 3));
}

void demo_expert() {
    const Matrix<int, 1, 2> m = [] {
        Matrix<int, 1, 2> t;
        t[0, 0] = 4;
        t[0, 1] = 5;
        return t;
    }();
    LEARN_CHECK((m[0, 1] == 5));
}

#else

template <class T, std::size_t Rows, std::size_t Cols>
class Matrix {
    std::array<T, Rows * Cols> data_{};

public:
    T& at(std::size_t r, std::size_t c) { return data_[r * Cols + c]; }
    const T& at(std::size_t r, std::size_t c) const { return data_[r * Cols + c]; }
};

void demo_basics() {
    Matrix<int, 2, 3> m;
    m.at(0, 0) = 1;
    LEARN_CHECK(m.at(0, 0) == 1);
}

void demo_intermediate() {
    LEARN_CHECK(true);
}

void demo_expert() {
    LEARN_CHECK(true);
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/multidim_operator_subscript_cpp23", run>;

}  // namespace
