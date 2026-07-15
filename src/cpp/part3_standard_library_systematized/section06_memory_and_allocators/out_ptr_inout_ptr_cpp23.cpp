// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : out_ptr_inout_ptr_cpp23
// Topic id : part3/section06/out_ptr_inout_ptr_cpp23
//
// Covers: std::out_ptr / inout_ptr for C API adoption

#include "learn/topic_registry.hpp"

#include <cassert>
#include <memory>
#include <version>

namespace {

void demo_basics() {
    // simulate C API: void create(int** out)
    auto create = [](int** out) { *out = new int{42}; };
    int* raw = nullptr;
    create(&raw);
    std::unique_ptr<int> p{raw};
    assert(*p == 42);
}

void demo_intermediate() {
#if defined(__cpp_lib_out_ptr) && __cpp_lib_out_ptr >= 202106L
    auto create = [](int** out) { *out = new int{7}; };
    std::unique_ptr<int> p;
    create(std::out_ptr(p));
    assert(p && *p == 7);
#else
    assert(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_out_ptr) && __cpp_lib_out_ptr >= 202106L
    auto replace = [](int** p) {
        delete *p;
        *p = new int{9};
    };
    std::unique_ptr<int> u = std::make_unique<int>(1);
    replace(std::inout_ptr(u));
    assert(u && *u == 9);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section06/out_ptr_inout_ptr_cpp23", run>;

}  // namespace
