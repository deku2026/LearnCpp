// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : install_export_find_package
// Topic id : part4/section01/install_export_find_package
//
// Covers: install/export Config.cmake find_package

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // install(TARGETS core EXPORT CoreTargets)
    std::string export_name = "CoreTargets";
    assert(!export_name.empty());
}

void demo_intermediate() {
    // generate CoreConfig.cmake with CMakePackageConfigHelpers
    assert(true);
}

void demo_expert() {
    // consumers: find_package(Core REQUIRED); target_link_libraries(app PRIVATE Core::core)
    std::string imported = "Core::core";
    assert(imported.find("::") != std::string::npos);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/install_export_find_package", run>;

}  // namespace
