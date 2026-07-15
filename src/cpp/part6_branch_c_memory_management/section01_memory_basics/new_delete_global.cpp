// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section01_memory_basics
// Item     : new_delete_global
// Topic id : part6/c/section01/new_delete_global
//
// Covers: global new/delete, new[], nothrow new

#include "learn/topic_registry.hpp"

#include <cassert>
#include <new>

namespace {

void demo_basics() {
    int* p = new int(5);
    assert(*p == 5);
    delete p;
}

void demo_intermediate() {
    int* a = new int[3]{1, 2, 3};
    assert(a[2] == 3);
    delete[] a;
}

void demo_expert() {
    int* p = new (std::nothrow) int(9);
    assert(p != nullptr);
    assert(*p == 9);
    delete p;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section01/new_delete_global", run>;

}  // namespace
