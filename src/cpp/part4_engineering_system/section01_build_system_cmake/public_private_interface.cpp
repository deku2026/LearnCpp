// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : public_private_interface
// Topic id : part4/section01/public_private_interface
//
// Covers: PUBLIC/PRIVATE/INTERFACE visibility propagation

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // PRIVATE: only this target; PUBLIC: this + consumers; INTERFACE: consumers only
    enum class Vis { Private, Public, Interface };
    assert(static_cast<int>(Vis::Public) != static_cast<int>(Vis::Private));
}

void demo_intermediate() {
    // Header dependency => PUBLIC/INTERFACE link; .cpp-only dep => PRIVATE
    const bool header_dep_is_public = true;
    assert(header_dep_is_public);
}

void demo_expert() {
    // INTERFACE libraries model header-only packages
    std::string kind = "INTERFACE";
    assert(kind == "INTERFACE");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/public_private_interface", run>;

}  // namespace
