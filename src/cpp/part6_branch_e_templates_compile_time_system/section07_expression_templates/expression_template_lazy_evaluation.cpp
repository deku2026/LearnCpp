// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section07_expression_templates
// Item     : expression_template_lazy_evaluation
// Topic id : part6/e/section07/expression_template_lazy_evaluation
//
// Covers: expression templates lazy evaluation sketch

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <vector>

namespace {

struct Vec {
    std::vector<double> data;
    explicit Vec(std::size_t n, double v = 0) : data(n, v) {}
    double operator[](std::size_t i) const { return data[i]; }
    std::size_t size() const { return data.size(); }
};

template <class L, class R>
struct AddExpr {
    const L& l;
    const R& r;
    double operator[](std::size_t i) const { return l[i] + r[i]; }
    std::size_t size() const { return l.size(); }
};

template <class L, class R>
AddExpr<L, R> operator+(const L& l, const R& r) {
    return {l, r};
}

template <class Expr>
Vec eval(const Expr& e) {
    Vec out(e.size());
    for (std::size_t i = 0; i < e.size(); ++i) out.data[i] = e[i];
    return out;
}

void demo_basics() {
    Vec a(3, 1.0), b(3, 2.0);
    auto e = a + b;
    assert(e[0] == 3.0);
}

void demo_intermediate() {
    Vec a(2, 1.0), b(2, 2.0), c(2, 3.0);
    Vec r = eval(a + b + c);
    assert(r[0] == 6.0);
}

void demo_expert() {
    // Lazy: no temporary Vec for intermediate sums until eval.
    Vec a(1, 5.0), b(1, 5.0);
    assert((a + b)[0] == 10.0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section07/expression_template_lazy_evaluation", run>;

}  // namespace
