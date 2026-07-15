// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : c_compat_headers
// Topic id : part3/section08/c_compat_headers
//
// Covers: headers map: cstdlib cstring cstdio cmath cstdint cassert

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

void demo_basics() {
    assert(std::abs(-2) == 2);
    assert(std::strlen("ab") == 2);
}

void demo_intermediate() {
    std::int32_t x = 1;
    assert(x == 1);
    char buf[8];
    std::snprintf(buf, sizeof(buf), "%d", 7);
    assert(buf[0] == '7');
}

void demo_expert() {
    void* p = std::malloc(16);
    assert(p != nullptr);
    std::free(p);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/c_compat_headers", run>;

}  // namespace
