// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : ninja_backend
// Topic id : part4/section01/ninja_backend
//
// Covers: Ninja as fast CMake backend

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // cmake -G Ninja -B build
    std::string generator = "Ninja";
    LEARN_CHECK(generator == "Ninja");
}

void demo_intermediate() {
    // Ninja is single-config; pass -DCMAKE_BUILD_TYPE=Debug
    LEARN_CHECK(true);
}

void demo_expert() {
    // Parallelism is default; good fit for CI
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/ninja_backend", run>;

}  // namespace
