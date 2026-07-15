// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section01_instantiation_model
// Item     : two_phase_lookup_recap
// Topic id : part6/e/section01/two_phase_lookup_recap
//
// Covers: two-phase lookup recap for templates

#include "learn/topic_registry.hpp"

namespace {

int free_val() {
    return 10;
}

template <class T>
struct Helper {
    static int get() { return 20; }
};

template <class T>
int combine(T) {
    return free_val() + Helper<T>::get();
}

void demo_basics() {
    LEARN_CHECK(combine(0) == 30);
}

void demo_intermediate() {
    LEARN_CHECK(combine(1.0) == 30);
}

void demo_expert() {
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section01/two_phase_lookup_recap", run>;

}  // namespace
