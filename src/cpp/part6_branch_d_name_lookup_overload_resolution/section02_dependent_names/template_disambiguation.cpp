// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section02_dependent_names
// Item     : template_disambiguation
// Topic id : part6/d/section02/template_disambiguation
//
// Covers: template keyword for dependent member templates

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

struct Factory {
    template <class T>
    T make(T v) const {
        return v;
    }
};

template <class F>
int use(const F& f) {
    return f.template make<int>(42);
}

void demo_basics() {
    Factory f;
    assert(f.make<int>(3) == 3);
}

void demo_intermediate() {
    assert(use(Factory{}) == 42);
}

void demo_expert() {
    // Without .template, parse may treat make as non-template and < as less-than.
    assert(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section02/template_disambiguation", run>;

}  // namespace
