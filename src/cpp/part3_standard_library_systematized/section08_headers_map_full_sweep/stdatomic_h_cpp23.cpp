// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : stdatomic_h_cpp23
// Topic id : part3/section08/stdatomic_h_cpp23
//
// Covers: C <stdatomic.h> compatibility in C++23 (prefer <atomic>)

#include "learn/topic_registry.hpp"

#include <atomic>
#include <cassert>
#include <version>

namespace {

void demo_basics() {
    std::atomic<int> x{0};
    x.store(1);
    assert(x.load() == 1);
}

void demo_intermediate() {
    // C++ code should prefer <atomic>; <stdatomic.h> is C interop (C++23)
#if defined(__cpp_lib_stdatomic_h) && __cpp_lib_stdatomic_h >= 202011L
    assert(__cpp_lib_stdatomic_h >= 202011L);
#else
    assert(true);
#endif
}

void demo_expert() {
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
    assert(!flag.test_and_set());
    flag.clear();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/stdatomic_h_cpp23", run>;

}  // namespace
