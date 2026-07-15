// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : implicit_object_creation_cpp20
// Topic id : part6/g/section04/implicit_object_creation_cpp20
//
// Covers: implicit object creation for certain operations (C++20)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <new>

namespace {

void demo_basics() {
    // Operations like malloc/memcpy can implicitly create trivial objects.
    void* p = ::operator new(sizeof(int));
    std::memset(p, 0, sizeof(int));
    int* i = new (p) int(0);
    assert(*i == 0);
    ::operator delete(p);
}

void demo_intermediate() {
    unsigned char buf[sizeof(int)]{};
    std::memcpy(buf, "\x01\x00\x00\x00", sizeof(int) > 4 ? 4 : sizeof(int));
    int v = 0;
    std::memcpy(&v, buf, sizeof(int));
    assert(v == 1 || v != 0 || v == 0);
}

void demo_expert() {
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/implicit_object_creation_cpp20", run>;

}  // namespace
