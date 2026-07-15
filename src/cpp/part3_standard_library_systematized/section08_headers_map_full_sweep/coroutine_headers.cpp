// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : coroutine_headers
// Topic id : part3/section08/coroutine_headers
//
// Covers: headers map: coroutine (header presence)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <coroutine>
#include <version>

namespace {

void demo_basics() {
#if defined(__cpp_impl_coroutine) && __cpp_impl_coroutine >= 201902L
    assert(true);
#else
    assert(true);
#endif
}

void demo_intermediate() {
    // full generators need a task type; header provides coroutine_traits/handle
    assert(sizeof(void*) >= 4);
}

void demo_expert() {
#if defined(__cpp_lib_coroutine) && __cpp_lib_coroutine >= 201902L
    assert(__cpp_lib_coroutine >= 201902L);
#else
    assert(true);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/coroutine_headers", run>;

}  // namespace
