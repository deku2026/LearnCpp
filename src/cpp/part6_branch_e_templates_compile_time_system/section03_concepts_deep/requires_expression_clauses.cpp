// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section03_concepts_deep
// Item     : requires_expression_clauses
// Topic id : part6/e/section03/requires_expression_clauses
//
// Covers: requires-expressions: simple, type, compound, nested

#include "learn/topic_registry.hpp"

#include <cassert>
#include <concepts>
#include <vector>

namespace {

template <class T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

template <class T>
concept HasNested = requires { typename T::value_type; };

struct IntWrap {
    int v;
    IntWrap operator+(IntWrap o) const { return IntWrap{v + o.v}; }
};

void demo_basics() {
    static_assert(Addable<int>);
    static_assert(Addable<IntWrap>);
}

void demo_intermediate() {
    static_assert(HasNested<std::vector<int>> || !HasNested<int>);
    IntWrap a{1}, b{2};
    assert((a + b).v == 3);
}

void demo_expert() {
    static_assert(requires(int x) {
        x + x;
        requires sizeof(x) >= 1;
    });
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section03/requires_expression_clauses", run>;

}  // namespace
