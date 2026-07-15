// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section01_function_basics
// Item     : inline_means_relax_odr
// Topic id : part2/stage03/section01/inline_means_relax_odr
//
// Covers: inline relaxes ODR (not force-inline); implicit inline cases

#include "learn/topic_registry.hpp"

#include <type_traits>

// Same definition may appear in multiple TUs when marked inline.
inline int add_inline(int a, int b) {
    return a + b;
}

inline int g_inline_counter = 0;

struct Point {
    int x = 0;
    int y = 0;
    // Class-body member definition is implicitly inline.
    int sum() const { return x + y; }
};

constexpr int square(int n) {
    return n * n;
}

template <class T>
T tmax(T a, T b) {
    return a > b ? a : b;
}

namespace {

void demo_basics() {
    LEARN_CHECK(add_inline(2, 3) == 5);
    ++g_inline_counter;
    LEARN_CHECK(g_inline_counter >= 1);

    Point p{3, 4};
    LEARN_CHECK(p.sum() == 7);
}

void demo_intermediate() {
    static_assert(square(5) == 25);
    LEARN_CHECK(square(6) == 36);
    LEARN_CHECK(tmax(1, 9) == 9);
    LEARN_CHECK(tmax(2.5, 1.5) == 2.5);
}

void demo_expert() {
    // inline is an ODR/linkage tool; optimizers may still ignore "inline expand".
    static_assert(std::is_same_v<decltype(square(1)), int>);
    const int before = g_inline_counter;
    g_inline_counter = before;
    LEARN_CHECK(g_inline_counter == before);
    LEARN_CHECK(add_inline(0, 0) == 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section01/inline_means_relax_odr", run>;

}  // namespace
