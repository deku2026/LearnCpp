// Topic     : C++23 多维下标 operator[]
// Doc       : 第2部分-阶段4 · 步骤 6.5
// cppreference: https://en.cppreference.com/cpp/language/operators
// 提案      : P2128
//
// 要点: operator[] 可接受多个参数；写 m[i, j]；是 mdspan 语言基础。

#include "learn/topic_registry.hpp"

#include <array>
#include <iostream>

namespace {

template <class T, std::size_t Rows, std::size_t Cols>
class Matrix {
    std::array<T, Rows * Cols> data_{};

public:
    T& operator[](std::size_t r, std::size_t c) { return data_[r * Cols + c]; }
    const T& operator[](std::size_t r, std::size_t c) const { return data_[r * Cols + c]; }

    static constexpr std::size_t rows() { return Rows; }
    static constexpr std::size_t cols() { return Cols; }
};

// 三维
template <class T, std::size_t A, std::size_t B, std::size_t C>
class Tensor3 {
    std::array<T, A * B * C> data_{};

public:
    T& operator[](std::size_t i, std::size_t j, std::size_t k) { return data_[(i * B + j) * C + k]; }
    const T& operator[](std::size_t i, std::size_t j, std::size_t k) const { return data_[(i * B + j) * C + k]; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [multidim_operator_subscript_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：2D m[i, j]
    // -------------------------------------------------------------------------
    Matrix<int, 2, 3> m;
    for (std::size_t i = 0; i < m.rows(); ++i) {
        for (std::size_t j = 0; j < m.cols(); ++j) {
            m[i, j] = static_cast<int>(i * m.cols() + j);
        }
    }
    // assert 是宏：含逗号的表达式必须再包一层括号
    assert((m[0, 0] == 0));
    assert((m[0, 2] == 2));
    assert((m[1, 0] == 3));
    assert((m[1, 2] == 5));
    std::cout << "[intro] m[1,2]=" << m[1, 2] << '\n';

    // -------------------------------------------------------------------------
    // §进阶：const 下标
    // -------------------------------------------------------------------------
    const auto& cm = m;
    assert((cm[1, 1] == 4));
    std::cout << "[advanced] const m[1,1]=" << cm[1, 1] << '\n';

    // -------------------------------------------------------------------------
    // §专家：三维 + C 数组逗号弃用提示
    // -------------------------------------------------------------------------
    Tensor3<int, 2, 2, 2> t;
    t[1, 0, 1] = 42;
    assert((t[1, 0, 1] == 42));

    // 内置数组 a[i, j] 在 C++23 起是被弃用的逗号表达式用法，勿再依赖
    // int a[2][2]{}; (void)a[0, 1];

    std::cout << "[expert] tensor[1,0,1]=" << t[1, 0, 1] << '\n';
    std::cout << "=== multidim_operator_subscript_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section05/multidim_operator_subscript_cpp23", run>;

}  // namespace
