// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : effective_type
// Topic id : part6/g/section04/effective_type
//
// Covers: effective type idea (C heritage) vs C++ object model

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <memory>
#include <new>

namespace {

void demo_basics() {
    // C++: objects have types; storage reuse needs lifetime rules.
    int x = 1;
    assert(x == 1);
}

void demo_intermediate() {
    alignas(float) unsigned char buf[sizeof(float)];
    float* f = new (buf) float(1.5f);
    assert(*f == 1.5f);
    std::destroy_at(f);
}

void demo_expert() {
    int a = 3;
    int b = 0;
    std::memcpy(&b, &a, sizeof(int));
    assert(b == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/effective_type", run>;

}  // namespace
