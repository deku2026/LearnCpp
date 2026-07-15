// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section02_dependent_names
// Item     : two_phase_lookup
// Topic id : part6/d/section02/two_phase_lookup
//
// Covers: two-phase lookup: non-dependent at definition, dependent at instantiation

#include "learn/topic_registry.hpp"

namespace {

int non_dep() {
    return 1;
}

template <class T>
struct Trait {
    static int value() { return 2; }
};

template <class T>
int compute(T) {
    int a = non_dep();          // phase 1
    int b = Trait<T>::value();  // phase 2 (dependent)
    return a + b;
}

void demo_basics() {
    LEARN_CHECK(compute(0) == 3);
}

void demo_intermediate() {
    LEARN_CHECK(compute(1.0) == 3);
}

void demo_expert() {
    // Dependent names are looked up again at instantiation in the template definition context
    // plus ADL for function calls with dependent args.
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section02/two_phase_lookup", run>;

}  // namespace
