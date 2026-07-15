// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : cppcheck_setup
// Topic id : part4/section03/cppcheck_setup
//
// Covers: cppcheck static analysis

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // cppcheck --enable=all --std=c++23
    std::string tool = "cppcheck";
    LEARN_CHECK(tool == "cppcheck");
}

void demo_intermediate() {
    // Complements clang-tidy; different false-positive profile
    LEARN_CHECK(true);
}

void demo_expert() {
    // Suppress carefully; prefer fixing root causes
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section03/cppcheck_setup", run>;

}  // namespace
