// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : overload_versus_specialization
// Topic id : part6/d/section03/overload_versus_specialization
//
// Covers: overloading vs explicit specialization of function templates

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

template <class T>
int tag(T) {
    return 0;
}

template <>
int tag<int>(int) {
    return 1;
}

int tag(double) {  // non-template overload
    return 2;
}

void demo_basics() {
    assert(tag(1) == 1);    // specialization
    assert(tag(1.0) == 2);  // overload
    assert(tag(std::string{}) == 0);
}

void demo_intermediate() {
    // Overloads are preferred via overload resolution; specializations only
    // apply after a primary template is selected.
    assert(tag(2.0f) == 0 || tag(2.0f) == 2);
}

void demo_expert() {
    assert(tag(0) == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/overload_versus_specialization", run>;

}  // namespace
