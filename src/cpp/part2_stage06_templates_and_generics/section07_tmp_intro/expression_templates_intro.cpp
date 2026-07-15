// Topic    : 表达式模板入门 —— 延迟求值消临时量（了解级）
// Doc      : 第2部分-阶段6 · 步骤 13.3
// Stage    : part2_stage06_templates_and_generics
// Section  : section07_tmp_intro
// Item     : expression_templates_intro
// Topic id : part2/stage06/section07/expression_templates_intro
// Refs     : 《C++ Templates: The Complete Guide》Expression Templates
//            Eigen / Blaze 设计思想

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 朴素 Vec 加法产生临时对象
// ---------------------------------------------------------------------------

struct NaiveVec {
    std::vector<double> data;
    explicit NaiveVec(std::size_t n, double v = 0.0) : data(n, v) {}
};

NaiveVec operator+(const NaiveVec& a, const NaiveVec& b) {
    // 每次 + 都分配新向量 → a+b+c 产生多个临时量
    NaiveVec r(a.data.size());
    for (std::size_t i = 0; i < r.data.size(); ++i) {
        r.data[i] = a.data[i] + b.data[i];
    }
    return r;
}

// ---------------------------------------------------------------------------
// §进阶 — 迷你表达式模板：把「加法表达式」编码进类型
// ---------------------------------------------------------------------------

template <typename L, typename R>
struct AddExpr {
    const L& lhs;
    const R& rhs;
    double operator[](std::size_t i) const { return lhs[i] + rhs[i]; }
    std::size_t size() const { return lhs.size(); }
};

struct Vec {
    std::vector<double> data;
    explicit Vec(std::size_t n, double v = 0.0) : data(n, v) {}
    double operator[](std::size_t i) const { return data[i]; }
    double& operator[](std::size_t i) { return data[i]; }
    std::size_t size() const { return data.size(); }

    // 从任意表达式求值写入（一次循环，无中间临时 Vec）
    template <typename Expr>
    Vec& operator=(const Expr& e) {
        for (std::size_t i = 0; i < data.size(); ++i) {
            data[i] = e[i];
        }
        return *this;
    }
};

template <typename L, typename R>
AddExpr<L, R> operator+(const L& a, const R& b) {
    return AddExpr<L, R>{a, b};
}

// ---------------------------------------------------------------------------
// §专家 — 为何是深水区；与现代替代方案
// ---------------------------------------------------------------------------

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [expression_templates_intro] 入门：朴素加法 ===\n";
    NaiveVec na(3, 1.0), nb(3, 2.0), nc(3, 3.0);
    NaiveVec nr = na + nb + nc;  // 多个临时 NaiveVec
    assert(nr.data[0] == 6.0);
    std::cout << "naive sum[0]=" << nr.data[0] << '\n';

    std::cout << "=== 进阶：延迟求值，单次循环 ===\n";
    Vec a(4, 1.0), b(4, 2.0), c(4, 3.0), r(4);
    // a+b+c 的类型类似 AddExpr<AddExpr<Vec,Vec>, Vec>，赋值时才算
    r = a + b + c;
    for (std::size_t i = 0; i < r.size(); ++i) {
        assert(r[i] == 6.0);
    }
    // 表达式类型在编译期编码运算树 → 可融合循环、避免临时堆分配
    [[maybe_unused]] auto expr = a + b;
    assert(expr[0] == 3.0);
    std::cout << "expr template r[0]=" << r[0] << '\n';

    std::cout << "=== 专家：定位与替代 ===\n";
    std::cout << "  用途: 数值库 (Eigen/Blaze) 消除 a+b+c 临时量\n";
    std::cout << "  代价: 类型难读、编译慢、错误信息吓人\n";
    std::cout << "  现代: 有时用 expression + lazy view / ranges /\n";
    std::cout << "        或手写融合循环；深水 TMP 见第 6 部分支线\n";
    std::cout << "=== expression_templates_intro: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section07/expression_templates_intro", run>;

}  // namespace
