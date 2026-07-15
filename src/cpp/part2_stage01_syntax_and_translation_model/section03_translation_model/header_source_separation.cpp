// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : header_source_separation
// Topic id : part2/stage01/section03/header_source_separation
//
// Covers: headers hold declarations; sources hold definitions (simulated in one TU)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace {

// ========== simulated math_utils.hpp ==========
// Include guard would wrap a real header:
// #ifndef LEARN_MATH_UTILS_HPP
// #define LEARN_MATH_UTILS_HPP
namespace math_utils {

int add(int a, int b);
int mul(int a, int b);

class Accumulator {
public:
    explicit Accumulator(int start = 0);
    void push(int v);
    int total() const;

private:
    int sum_;
    int count_;
};

}  // namespace math_utils
// #endif  // LEARN_MATH_UTILS_HPP
// ========== end simulated header ==========

// ========== simulated math_utils.cpp ==========
// Real project: this block lives in math_utils.cpp and #includes "math_utils.hpp".
namespace math_utils {

int add(int a, int b) {
    return a + b;
}

int mul(int a, int b) {
    return a * b;
}

Accumulator::Accumulator(int start) : sum_(start), count_(0) {}

void Accumulator::push(int v) {
    sum_ += v;
    ++count_;
}

int Accumulator::total() const {
    return sum_;
}

}  // namespace math_utils
// ========== end simulated source ==========

// ========== simulated consumer.cpp ==========
// Real project: another .cpp includes only the header and links against math_utils.o.
void demo_basics() {
    assert(math_utils::add(2, 3) == 5);
    assert(math_utils::mul(4, 5) == 20);
}

void demo_intermediate() {
    math_utils::Accumulator acc{10};
    acc.push(1);
    acc.push(2);
    acc.push(3);
    assert(acc.total() == 16);

    // Headers should not define non-inline free functions (would ODR-break if
    // included from multiple TUs). Definitions stay in the .cpp above.
    std::vector<int> xs{1, 2, 3};
    int s = 0;
    for (int x : xs) {
        s = math_utils::add(s, x);
    }
    assert(s == 6);
}

void demo_expert() {
    // Separation checklist (teaching):
    // .hpp  — declarations, inline/template definitions, constexpr, types
    // .cpp  — non-inline function/variable definitions, private helpers
    // Build — each .cpp → .o independently; linker merges symbols
    //
    // Why separate?
    // - Faster rebuilds: change .cpp → recompile one object, not every includer
    // - Hide implementation details (private members still visible in class,
    //   but free-function bodies and static helpers stay out of headers)
    // - Clear ownership of the single definition required by ODR

    math_utils::Accumulator acc;
    for (int i = 1; i <= 5; ++i) {
        acc.push(math_utils::mul(i, i));
    }
    // 1+4+9+16+25 = 55
    assert(acc.total() == 55);

    const std::string iface = "math_utils.hpp";
    const std::string impl = "math_utils.cpp";
    assert(iface.ends_with(".hpp"));
    assert(impl.ends_with(".cpp"));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/header_source_separation", run>;

}  // namespace
