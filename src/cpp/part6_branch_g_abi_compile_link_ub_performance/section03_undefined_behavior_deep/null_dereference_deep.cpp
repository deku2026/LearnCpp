// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : null_dereference_deep
// Topic id : part6/g/section03/null_dereference_deep
//
// Covers: null dereference is UB — check pointers; prefer references

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>

namespace {

int read(const int* p) {
    if (!p) return -1;
    return *p;
}

void demo_basics() {
    int x = 3;
    assert(read(&x) == 3);
    assert(read(nullptr) == -1);
}

void demo_intermediate() {
    auto p = std::make_unique<int>(9);
    assert(read(p.get()) == 9);
}

void demo_expert() {
    // Not executed: *static_cast<int*>(nullptr);
    auto owner = std::make_unique<int>(1);
    const int& ok = *owner;
    assert(ok == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/null_dereference_deep", run>;

}  // namespace
