// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section04_lifetime_routines
// Item     : start_lifetime_as_cpp23
// Topic id : part6/c/section04/start_lifetime_as_cpp23
//
// Covers: std::start_lifetime_as for implicit-lifetime types (C++23)

#include "learn/topic_registry.hpp"

#include <cstring>
#include <new>
#include <type_traits>

#if defined(__has_include)
#if __has_include(<memory>)
#include <memory>
#endif
#endif
namespace {

void demo_basics() {
    alignas(int) unsigned char buf[sizeof(int)]{};
#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L && __has_include(<memory>)
    int* p = std::start_lifetime_as<int>(buf);
    *p = 42;
    LEARN_CHECK(*p == 42);
#else
    // Portable fallback: placement new starts lifetime of int.
    int* p = new (buf) int(42);
    LEARN_CHECK(*p == 42);
#endif
}

void demo_intermediate() {
    struct Agg {
        int x;
        int y;
    };
    alignas(Agg) unsigned char buf[sizeof(Agg)]{};
#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L && __has_include(<memory>)
    Agg* a = std::start_lifetime_as<Agg>(buf);
    a->x = 1;
    a->y = 2;
    LEARN_CHECK(a->x + a->y == 3);
#else
    Agg* a = new (buf) Agg{1, 2};
    LEARN_CHECK(a->x + a->y == 3);
#endif
}

void demo_expert() {
    // Only for implicit-lifetime / trivial cases; not a general type-pun tool.
    static_assert(std::is_trivially_copyable_v<int>);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section04/start_lifetime_as_cpp23", run>;

}  // namespace
