// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section07_import_std_exception
// Item     : header_versus_module_versus_import_std
// Topic id : part4/section07/header_versus_module_versus_import_std
//
// Covers: headers vs named modules vs import std tradeoffs

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

void demo_basics() {
    // headers: ubiquitous; modules: faster rebuilds potential; import std: std as module
    const char* styles[] = {"header", "module", "import_std"};
    assert(std::string_view{styles[0]} == "header");
}

void demo_intermediate() {
    // Migration path: headers -> modules for project code; std modules last
    assert(true);
}

void demo_expert() {
    // This repository remains header-based for maximum runnable coverage
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section07/header_versus_module_versus_import_std", run>;

}  // namespace
