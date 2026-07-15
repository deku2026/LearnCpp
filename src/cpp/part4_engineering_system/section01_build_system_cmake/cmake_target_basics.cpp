// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : cmake_target_basics
// Topic id : part4/section01/cmake_target_basics
//
// Covers: CMake target-based mental model as code comments + asserts

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // add_library(core ...); add_executable(app main.cpp); target_link_libraries(app PRIVATE core)
    std::string target = "core";
    LEARN_CHECK(target == "core");
}

void demo_intermediate() {
    // Prefer target_compile_features(core PUBLIC cxx_std_23) over global CMAKE_CXX_FLAGS
    const bool target_based = true;
    LEARN_CHECK(target_based);
}

void demo_expert() {
    // One target = one artifact with its own includes/options/deps
    LEARN_CHECK(std::string_view{"myapp"} != "global_flags");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/cmake_target_basics", run>;

}  // namespace
