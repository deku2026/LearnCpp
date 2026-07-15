// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section02_package_managers
// Item     : conan_profile
// Topic id : part4/section02/conan_profile
//
// Covers: Conan profiles and lockfiles

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // conan install . -pr:h default -pr:b default
    std::string profile = "default";
    assert(!profile.empty());
}

void demo_intermediate() {
    // settings: os/compiler/build_type/arch
    assert(true);
}

void demo_expert() {
    // conan.lock freezes graph
    std::string lock = "conan.lock";
    assert(lock.find("lock") != std::string::npos);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section02/conan_profile", run>;

}  // namespace
