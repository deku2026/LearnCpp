// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section07_import_std_exception
// Item     : import_std_compat_cpp23
// Topic id : part4/section07/import_std_compat_cpp23
//
// Covers: import std compatibility constraints (no import std in file)

#include "learn/topic_registry.hpp"

#include <string>

namespace {

void demo_basics() {
    // Mixing import std and #include <vector> rules are implementation-sensitive
    std::string mode = "header-only-demo";
    LEARN_CHECK(mode.find("header") != std::string::npos);
}

void demo_intermediate() {
    // Prefer all-module or all-header per TU during transition
    LEARN_CHECK(true);
}

void demo_expert() {
    // Standard library modules still maturing across GCC/Clang/MSVC
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section07/import_std_compat_cpp23", run>;

}  // namespace
