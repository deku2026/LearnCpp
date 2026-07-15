// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section02_package_managers
// Item     : cpm_lightweight
// Topic id : part4/section02/cpm_lightweight
//
// Covers: CPM.cmake FetchContent wrapper

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // CPMAddPackage("gh:user/repo#tag")
    std::string src = "FetchContent";
    assert(!src.empty());
}

void demo_intermediate() {
    // Good for small header libs; less strict than vcpkg/Conan binary caching
    assert(true);
}

void demo_expert() {
    // Pin commits/tags; avoid floating main branches in CI
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section02/cpm_lightweight", run>;

}  // namespace
