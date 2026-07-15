// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section03_concepts_deep
// Item     : constraint_subsumption
// Topic id : part6/e/section03/constraint_subsumption
//
// Covers: concept subsumption for partial ordering

#include "learn/topic_registry.hpp"

#include <concepts>

namespace {

template <class T>
concept Integral = std::integral<T>;

template <class T>
concept SignedIntegral = Integral<T> && std::is_signed_v<T>;

template <Integral T>
int rank(T) {
    return 1;
}

template <SignedIntegral T>
int rank(T) {
    return 2;
}

void demo_basics() {
    LEARN_CHECK(rank(1) == 2);   // signed int
    LEARN_CHECK(rank(1u) == 1);  // unsigned
}

void demo_intermediate() {
    LEARN_CHECK(rank(static_cast<short>(1)) == 2);
}

void demo_expert() {
    static_assert(SignedIntegral<int>);
    static_assert(!SignedIntegral<unsigned>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section03/constraint_subsumption", run>;

}  // namespace
