// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section04_lifetime_routines
// Item     : start_lifetime_as_array_cpp23
// Topic id : part6/c/section04/start_lifetime_as_array_cpp23
//
// Covers: std::start_lifetime_as_array (C++23)

#include "learn/topic_registry.hpp"

#include <new>

#if defined(__has_include)
#if __has_include(<memory>)
#include <memory>
#endif
#endif
namespace {

void demo_basics() {
    alignas(int) unsigned char buf[sizeof(int) * 4]{};
#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L && __has_include(<memory>)
    int* a = std::start_lifetime_as_array<int>(buf, 4);
    a[0] = 1;
    a[3] = 4;
    LEARN_CHECK(a[0] == 1 && a[3] == 4);
#else
    int* a = new (buf) int[4];
    a[0] = 1;
    a[3] = 4;
    LEARN_CHECK(a[0] == 1 && a[3] == 4);
#endif
}

void demo_intermediate() {
    alignas(int) unsigned char buf[sizeof(int) * 2]{};
#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L && __has_include(<memory>)
    int* a = std::start_lifetime_as_array<int>(buf, 2);
#else
    int* a = reinterpret_cast<int*>(buf);
    new (a) int(0);
    new (a + 1) int(0);
#endif
    a[0] = 5;
    a[1] = 6;
    LEARN_CHECK(a[0] + a[1] == 11);
}

void demo_expert() {
    LEARN_CHECK(sizeof(int) * 4 >= 4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section04/start_lifetime_as_array_cpp23", run>;

}  // namespace
