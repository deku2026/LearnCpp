// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : expression_template_idiom
// Topic id : part2/stage15/section03/expression_template_idiom
//
// Covers: tiny expression template

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <utility>

namespace {

template <class L, class R>
struct AddExpr {
    const L& l;
    const R& r;
    int operator[](std::size_t i) const { return l[i] + r[i]; }
};

struct Vec {
    int d[3];
    int operator[](std::size_t i) const { return d[i]; }
    int& operator[](std::size_t i) { return d[i]; }
    template <class E>
    Vec& operator=(const E& e) {
        for (std::size_t i = 0; i < 3; ++i) {
            d[i] = e[i];
        }
        return *this;
    }
};

template <class L, class R>
AddExpr<L, R> operator+(const L& l, const R& r) {
    return AddExpr<L, R>{l, r};
}

void demo_basics() {
    Vec a{{1, 2, 3}};
    Vec b{{4, 5, 6}};
    Vec c{{0, 0, 0}};
    c = a + b;
    assert(c[0] == 5 && c[1] == 7 && c[2] == 9);
}

void demo_intermediate() {
    Vec a{{1, 1, 1}};
    Vec b{{2, 2, 2}};
    Vec c{{3, 3, 3}};
    Vec d{{0, 0, 0}};
    d = a + b + c;
    assert(d[0] == 6);
}

void demo_expert() {
    // Expression templates delay evaluation to avoid temporaries.
    Vec a{{1, 0, 0}};
    Vec b{{0, 1, 0}};
    auto e = a + b;
    assert(e[0] == 1 && e[1] == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section03/expression_template_idiom", run>;

}  // namespace
