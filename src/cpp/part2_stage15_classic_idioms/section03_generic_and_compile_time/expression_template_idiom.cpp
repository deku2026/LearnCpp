// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §3.5
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : expression_template_idiom
// Topic id : part2/stage15/section03/expression_template_idiom
//
// Expression templates: encode expression structure in types, evaluate once.
// Motive : avoid temporaries in chained vector/math expressions (a+b+c).
// Modern : Eigen/Blaze use this; app code should use those libs, not reinvent.
// Pitfall: lifetime of leaves (do not bind expressions to destroyed temporaries);
//          complexity and compile times are high.

#include "learn/topic_registry.hpp"

#include <concepts>
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <vector>

namespace {

// ---------- eager baseline ----------
struct EagerVec {
    std::vector<double> data;

    explicit EagerVec(std::size_t n, double v = 0.0) : data(n, v) {}

    std::size_t size() const { return data.size(); }
    double operator[](std::size_t i) const { return data[i]; }
    double& operator[](std::size_t i) { return data[i]; }
};

EagerVec operator+(const EagerVec& a, const EagerVec& b) {
    assert(a.size() == b.size());
    EagerVec r(a.size());
    for (std::size_t i = 0; i < a.size(); ++i) {
        r[i] = a[i] + b[i];
    }
    std::cout << "  [eager] temporary of size " << r.size() << " materialised\n";
    return r;  // a+b+c creates two temporaries
}

// ---------- expression templates ----------
template <typename L, typename R>
struct AddExpr {
    const L& lhs;
    const R& rhs;

    std::size_t size() const {
        assert(lhs.size() == rhs.size());
        return lhs.size();
    }

    double operator[](std::size_t i) const { return lhs[i] + rhs[i]; }
};

struct LazyVec {
    std::vector<double> data;

    explicit LazyVec(std::size_t n, double v = 0.0) : data(n, v) {}

    std::size_t size() const { return data.size(); }
    double operator[](std::size_t i) const { return data[i]; }
    double& operator[](std::size_t i) { return data[i]; }

    // Assign from any expression-like object: single pass, no intermediate vectors.
    template <typename Expr>
    LazyVec& operator=(const Expr& expr) {
        assert(expr.size() == size());
        std::cout << "  [lazy] single evaluation pass, n=" << size() << '\n';
        for (std::size_t i = 0; i < size(); ++i) {
            data[i] = expr[i];
        }
        return *this;
    }
};

// Constrained so we do not steal EagerVec's operator+.
template <typename T>
concept LazyLeafOrExpr = requires(const T& t, std::size_t i) {
    { t.size() } -> std::convertible_to<std::size_t>;
    { t[i] } -> std::convertible_to<double>;
} && !std::same_as<T, EagerVec>;

template <LazyLeafOrExpr L, LazyLeafOrExpr R>
AddExpr<L, R> operator+(const L& a, const R& b) {
    return AddExpr<L, R>{a, b};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== eager a+b+c (temporaries) ==\n";
    EagerVec ea(3, 1.0), eb(3, 2.0), ec(3, 3.0);
    EagerVec er = ea + eb + ec;  // two + ops → two temporaries
    assert(er[0] == 6.0);
    std::cout << "  result[0]=" << er[0] << '\n';

    std::cout << "== lazy expression templates ==\n";
    LazyVec a(4, 1.0), b(4, 2.0), c(4, 3.0), d(4, 4.0);
    LazyVec r(4);
    // Type of a+b+c+d is nested AddExpr<...>, evaluation only on assign.
    r = a + b + c + d;
    assert(r[0] == 10.0);
    assert(r[3] == 10.0);
    std::cout << "  result[0]=" << r[0] << " (one pass over 4 terms)\n";

    std::cout << "trap: auto e = a+b; then use e after a dies → dangling refs\n";
    std::cout << "use Eigen/Blaze in production; this is a teaching sketch only\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section03/expression_template_idiom", run>;

}  // namespace
