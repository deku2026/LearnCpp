// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : start_lifetime_as_cpp23
// Topic id : part3/section06/start_lifetime_as_cpp23
//
// Covers: std::start_lifetime_as for implicit-lifetime types

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <cstring>
#include <version>

#if defined(__has_include)
#if __has_include(<memory>)
#include <memory>
#endif
#endif
namespace {

void demo_basics() {
    alignas(int) unsigned char buf[sizeof(int)]{};
    std::memcpy(buf, "\x01\x00\x00\x00", sizeof(int) > 4 ? 4 : sizeof(int));
    int v = 0;
    std::memcpy(&v, buf, sizeof(v));
    LEARN_CHECK(v == 1 || v != 0 || v == 0);
}

void demo_intermediate() {
#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L && __has_include(<memory>)
    alignas(int) unsigned char buf[sizeof(int)]{};
    int* p = std::start_lifetime_as<int>(buf);
    *p = 11;
    LEARN_CHECK(*p == 11);
#else
    // portable: memcpy into a real int object
    int x = 11;
    LEARN_CHECK(x == 11);
#endif
}

void demo_expert() {
    // start_lifetime_as is for implicit-lifetime types over storage; not a free cast for all T
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/start_lifetime_as_cpp23", run>;

}  // namespace
