// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section06_crtp
// Item     : crtp_deducing_this_simplification_cpp23
// Topic id : part2/stage06/section06/crtp_deducing_this_simplification_cpp23
//
// Covers: C++23 deducing this can replace classic CRTP boilerplate

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <utility>
#include <version>

namespace {

// Classic CRTP
template <typename D>
struct AddableCrtp {
    D operator+(const D& other) const {
        D tmp = static_cast<const D&>(*this);
        tmp.add(other);
        return tmp;
    }
};

struct Num : AddableCrtp<Num> {
    int v = 0;
    void add(const Num& o) { v += o.v; }
};

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
struct Num23 {
    int v = 0;
    template <typename Self>
    auto operator+(this Self&& self, const Num23& other) {
        Num23 out{self.v + other.v};
        return out;
    }
};
#endif

void demo_basics() {
    Num a;
    a.v = 2;
    Num b;
    b.v = 3;
    Num c = a + b;
    assert(c.v == 5);
}

void demo_intermediate() {
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    Num23 a{10};
    Num23 b{32};
    auto c = a + b;
    assert(c.v == 42);
#else
    // Portable fallback: classic CRTP path already demonstrated.
    Num a;
    a.v = 10;
    Num b;
    b.v = 32;
    assert((a + b).v == 42);
#endif
}

void demo_expert() {
    // Deduction of cv/ref qualifiers is the main win of deducing this;
    // without it, CRTP still provides static injection of operators.
    Num x;
    x.v = 1;
    Num y;
    y.v = 1;
    assert((x + y).v == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section06/crtp_deducing_this_simplification_cpp23", run>;

}  // namespace
