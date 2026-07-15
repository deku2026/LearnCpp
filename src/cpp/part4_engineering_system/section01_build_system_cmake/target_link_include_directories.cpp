// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : target_link_include_directories
// Topic id : part4/section01/target_link_include_directories
//
// Covers: target_link_libraries and target_include_directories

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // target_include_directories(core PUBLIC include)
    std::string include_dir = "include";
    assert(!include_dir.empty());
}

void demo_intermediate() {
    // $<BUILD_INTERFACE:...> vs $<INSTALL_INTERFACE:...> for exportable targets
    assert(true);
}

void demo_expert() {
    // Never use include_directories() globally in modern CMake
    const bool use_target_include = true;
    assert(use_target_include);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/target_link_include_directories", run>;

}  // namespace
