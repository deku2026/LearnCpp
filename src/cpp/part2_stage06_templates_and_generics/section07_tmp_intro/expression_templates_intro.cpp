// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section07_tmp_intro
// Item     : expression_templates_intro
// Topic id : part2/stage06/section07/expression_templates_intro
//
// Covers: tiny expression-template intro (lazy vector add without temps)

#include "learn/topic_registry.hpp"

#include <array>
#include <cstddef>

namespace {

template <typename L, typename R>
struct AddExpr {
    const L& lhs;
    const R& rhs;
    auto operator[](std::size_t i) const { return lhs[i] + rhs[i]; }
};

template <typename L, typename R>
AddExpr<L, R> operator+(const L& a, const R& b) {
    return AddExpr<L, R>{a, b};
}

struct Vec3 {
    std::array<double, 3> d{};
    double operator[](std::size_t i) const { return d[i]; }
    double& operator[](std::size_t i) { return d[i]; }

    template <typename Expr>
    Vec3& operator=(const Expr& e) {
        for (std::size_t i = 0; i < 3; ++i) {
            d[i] = e[i];
        }
        return *this;
    }
};

void demo_basics() {
    Vec3 a;
    a.d = {1, 2, 3};
    Vec3 b;
    b.d = {4, 5, 6};
    Vec3 c;
    c = a + b;  // single loop assignment, no intermediate Vec3 from +
    LEARN_CHECK(c[0] == 5);
    LEARN_CHECK(c[1] == 7);
    LEARN_CHECK(c[2] == 9);
}

void demo_intermediate() {
    Vec3 a{{1, 1, 1}};
    Vec3 b{{2, 2, 2}};
    Vec3 c{{3, 3, 3}};
    Vec3 d;
    d = a + b + c;  // nested AddExpr
    LEARN_CHECK(d[0] == 6);
    LEARN_CHECK(d[2] == 6);
}

void demo_expert() {
    Vec3 a{{10, 0, 0}};
    Vec3 b{{0, 20, 0}};
    auto expr = a + b;
    LEARN_CHECK(expr[0] == 10);
    LEARN_CHECK(expr[1] == 20);
    LEARN_CHECK(expr[2] == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section07/expression_templates_intro", run>;

}  // namespace
