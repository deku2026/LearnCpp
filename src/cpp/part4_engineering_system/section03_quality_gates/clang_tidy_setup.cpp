// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : clang_tidy_setup
// Topic id : part4/section03/clang_tidy_setup
//
// Covers: clang-tidy with compile_commands.json

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // CMAKE_EXPORT_COMPILE_COMMANDS=ON
    std::string db = "compile_commands.json";
    LEARN_CHECK(db.ends_with(".json"));
}

void demo_intermediate() {
    // Checks: bugprone-*,clang-analyzer-*,modernize-*
    LEARN_CHECK(true);
}

void demo_expert() {
    // warnings-as-errors for selected checks in CI
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section03/clang_tidy_setup", run>;

}  // namespace
