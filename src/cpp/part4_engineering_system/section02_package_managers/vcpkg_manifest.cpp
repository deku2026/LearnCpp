// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section02_package_managers
// Item     : vcpkg_manifest
// Topic id : part4/section02/vcpkg_manifest
//
// Covers: vcpkg.json manifest mode

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // { "dependencies": ["fmt", "gtest"] }
    std::string manifest = "vcpkg.json";
    assert(manifest.ends_with(".json"));
}

void demo_intermediate() {
    // cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg]/scripts/buildsystems/vcpkg.cmake
    assert(true);
}

void demo_expert() {
    // baseline/builtin-baseline pins registry for reproducibility
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section02/vcpkg_manifest", run>;

}  // namespace
