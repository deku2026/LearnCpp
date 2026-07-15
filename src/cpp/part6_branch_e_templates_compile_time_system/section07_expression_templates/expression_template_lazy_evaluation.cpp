// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E7 expression templates)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section07_expression_templates
// Item     : expression_template_lazy_evaluation
// Topic id : part6/e/section07/expression_template_lazy_evaluation
//
// 要点: 表达式模板把 a+b+c 编码成类型树, 一次循环求值, 避免临时向量。
// 参考: Veldhuizen; Eigen; Boost.uBLAS; C++ Templates Complete Guide

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

namespace {

// 向量引用叶子
struct VecRef {
    const std::vector<double>& v;
    double operator[](std::size_t i) const { return v[i]; }
    std::size_t size() const { return v.size(); }
};

// 二元加法表达式节点
template <typename L, typename R>
struct AddExpr {
    L left;
    R right;
    double operator[](std::size_t i) const { return left[i] + right[i]; }
    std::size_t size() const { return left.size(); }
};

template <typename L, typename R>
AddExpr<L, R> operator+(L l, R r) {
    return {std::move(l), std::move(r)};
}

// 目标向量: 从表达式一次赋值
struct Vec {
    std::vector<double> data;

    explicit Vec(std::size_t n, double fill = 0.0) : data(n, fill) {}
    explicit Vec(std::vector<double> v) : data(std::move(v)) {}

    double& operator[](std::size_t i) { return data[i]; }
    double operator[](std::size_t i) const { return data[i]; }
    std::size_t size() const { return data.size(); }

    VecRef ref() const { return VecRef{data}; }

    template <typename Expr>
    Vec& operator=(const Expr& e) {
        assert(e.size() == data.size());
        for (std::size_t i = 0; i < data.size(); ++i) {
            data[i] = e[i];  // 单次遍历, 无临时 Vec
        }
        return *this;
    }
};

// 方便: Vec + Vec → 表达式 (经 ref)
inline auto operator+(const Vec& a, const Vec& b) {
    return a.ref() + b.ref();
}

template <typename E>
auto operator+(const Vec& a, E e) {
    return a.ref() + std::move(e);
}

template <typename E>
auto operator+(E e, const Vec& b) {
    return std::move(e) + b.ref();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E7 expression templates (lazy) ===\n";

    Vec a(4, 1.0);
    Vec b(4, 2.0);
    Vec c(4, 3.0);
    Vec d(4, 0.0);

    // 朴素: d = a+b+c 可能产生多个临时向量
    // 表达式模板: 类型为 AddExpr<AddExpr<VecRef,VecRef>, VecRef>
    d = a + b + c;

    for (std::size_t i = 0; i < d.size(); ++i) {
        assert(d[i] == 6.0);
    }

    // 部分求值演示: 表达式可延迟
    auto expr = a.ref() + b.ref();
    assert(expr[0] == 3.0);

    Vec e(4, 0.0);
    e = expr + c.ref();
    assert(e[0] == 6.0);

    std::cout << "  a+b+c fused into one loop; no per-op temporary vectors\n";
    std::cout << "  used by Eigen / Blaze / numeric DSLs\n";
    std::cout << "expression_template_lazy_evaluation: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section07/expression_template_lazy_evaluation", run>;

}  // namespace
