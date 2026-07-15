// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : start_lifetime_as_in_systems_code_cpp23
// Topic id : part6/g/section04/start_lifetime_as_in_systems_code_cpp23
//
// Covers: start_lifetime_as in systems/byte-buffer code

#include "learn/topic_registry.hpp"

#include <cassert>
#include <new>

namespace {

void demo_basics() {
    alignas(int) unsigned char buf[sizeof(int)]{};
#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L
    int* p = std::start_lifetime_as<int>(buf);
    *p = 5;
    assert(*p == 5);
#else
    int* p = new (buf) int(5);
    assert(*p == 5);
#endif
}

void demo_intermediate() {
    alignas(int) unsigned char buf[sizeof(int) * 2]{};
#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L
    int* a = std::start_lifetime_as_array<int>(buf, 2);
    a[0] = 1;
    a[1] = 2;
    assert(a[0] + a[1] == 3);
#else
    int* a = new (buf) int[2];
    a[0] = 1;
    a[1] = 2;
    assert(a[0] + a[1] == 3);
#endif
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/start_lifetime_as_in_systems_code_cpp23", run>;

}  // namespace
