// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section07_import_std_exception
// Item     : import_std_basic_cpp23
// Topic id : part4/section07/import_std_basic_cpp23
//
// Covers: import std conceptual — header-based gated demo only (no import std)

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

namespace {

void demo_basics() {
    // Portable equivalent of std module import for this course tree:
    std::string s = "headers";
    LEARN_CHECK(s == "headers");
}

void demo_intermediate() {
    // Real import std needs module-enabled toolchain + BMI build rules
#if defined(__cpp_lib_modules)
    LEARN_CHECK(true);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
    // Keep demos header-based so all CI compilers can run topics
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section07/import_std_basic_cpp23", run>;

}  // namespace
