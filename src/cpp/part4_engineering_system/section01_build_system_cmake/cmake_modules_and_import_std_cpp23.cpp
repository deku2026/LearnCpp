// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : cmake_modules_and_import_std_cpp23
// Topic id : part4/section01/cmake_modules_and_import_std_cpp23
//
// Covers: CMake C++ modules / import std notes (conceptual)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // CMake 3.28+ experimental/module support evolving
    assert(true);
}

void demo_intermediate() {
    // import std requires toolchain BMI/module map setup
    // This repo demos stay header-based for portability
    const bool header_based_demo = true;
    assert(header_based_demo);
}

void demo_expert() {
    // Do not mix import std with conflicting header inclusion carelessly
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/cmake_modules_and_import_std_cpp23", run>;

}  // namespace
