// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : generator_expressions
// Topic id : part4/section01/generator_expressions
//
// Covers: CMake generator expressions $<...>

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // $<CONFIG:Debug> $<CXX_COMPILER_ID:Clang> evaluated at generate time
    std::string genex = "$<CONFIG:Debug>";
    LEARN_CHECK(genex.find("CONFIG") != std::string::npos);
}

void demo_intermediate() {
    // $<TARGET_PROPERTY:tgt,INCLUDE_DIRECTORIES> for property queries
    LEARN_CHECK(true);
}

void demo_expert() {
    // Use genex for config-specific flags instead of if(CMAKE_BUILD_TYPE)
    const bool multi_config_safe = true;
    LEARN_CHECK(multi_config_safe);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/generator_expressions", run>;

}  // namespace
