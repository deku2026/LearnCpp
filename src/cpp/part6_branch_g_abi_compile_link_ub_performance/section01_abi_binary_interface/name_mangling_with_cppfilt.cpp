// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : name_mangling_with_cppfilt
// Topic id : part6/g/section01/name_mangling_with_cppfilt
//
// Covers: C++ name mangling; extern C disables it

#include "learn/topic_registry.hpp"

#include <typeinfo>

namespace {

namespace demo {
int add(int a, int b) {
    return a + b;
}
extern "C" int c_add(int a, int b) {
    return a + b;
}
}  // namespace demo

void demo_basics() {
    LEARN_CHECK(demo::add(1, 2) == 3);
    LEARN_CHECK(demo::c_add(1, 2) == 3);
}

void demo_intermediate() {
    LEARN_CHECK(typeid(int).name() != nullptr);
}

void demo_expert() {
    LEARN_CHECK(demo::add(2, 2) == demo::c_add(2, 2));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/name_mangling_with_cppfilt", run>;

}  // namespace
