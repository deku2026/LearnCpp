// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section07_import_std_exception
// Item     : import_std_ecosystem_caveats
// Topic id : part4/section07/import_std_ecosystem_caveats
//
// Covers: ecosystem caveats for std modules

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

void demo_basics() {
    std::string caveats = "tooling,BMI,cache,IDE";
    assert(caveats.find("BMI") != std::string::npos);
}

void demo_intermediate() {
    // Package managers and sanitizers may lag module support
    assert(true);
}

void demo_expert() {
    // Document supported compilers before adopting import std in production
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section07/import_std_ecosystem_caveats", run>;

}  // namespace
