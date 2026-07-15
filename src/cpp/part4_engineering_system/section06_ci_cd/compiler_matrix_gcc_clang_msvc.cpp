// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : compiler_matrix_gcc_clang_msvc
// Topic id : part4/section06/compiler_matrix_gcc_clang_msvc
//
// Covers: CI compilers GCC/Clang/MSVC

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // Catch non-portable code early
    const char* compilers[] = {"gcc", "clang", "msvc"};
    assert(std::string_view{compilers[0]} == "gcc");
}

void demo_intermediate() {
    // Same standard flag: -std=c++23 /std:c++latest
    assert(true);
}

void demo_expert() {
    // Treat warnings consistently per toolchain
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/compiler_matrix_gcc_clang_msvc", run>;

}  // namespace
