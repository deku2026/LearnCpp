// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : declspec_dllexport
// Topic id : part6/g/section01/declspec_dllexport
//
// Covers: dllexport/dllimport style annotations

#include "learn/topic_registry.hpp"

#if defined(_WIN32)
#define DEMO_EXPORT __declspec(dllexport)
#else
#define DEMO_EXPORT
#endif

// dllexport requires external linkage (not anonymous-namespace).
DEMO_EXPORT int dll_style_answer() {
    return 42;
}

namespace {

void demo_basics() {
    LEARN_CHECK(dll_style_answer() == 42);
}

void demo_intermediate() {
    LEARN_CHECK(true);
}

void demo_expert() {
    using Fn = int (*)();
    Fn f = &dll_style_answer;
    LEARN_CHECK(f() == 42);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/declspec_dllexport", run>;

}  // namespace
