// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : partial_ordering_of_function_templates
// Topic id : part6/d/section03/partial_ordering_of_function_templates
//
// Covers: partial ordering picks more specialized function template

#include "learn/topic_registry.hpp"

namespace {

template <class T>
int rank(T) {
    return 1;
}

template <class T>
int rank(T*) {
    return 2;
}

template <class T>
int rank(const T*) {
    return 3;
}

void demo_basics() {
    int x = 0;
    LEARN_CHECK(rank(x) == 1);
    LEARN_CHECK(rank(&x) == 2);
}

void demo_intermediate() {
    const int x = 0;
    LEARN_CHECK(rank(&x) == 3);
}

void demo_expert() {
    int* p = nullptr;
    LEARN_CHECK(rank(p) == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/partial_ordering_of_function_templates", run>;

}  // namespace
