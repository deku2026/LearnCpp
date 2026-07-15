// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : clang_format_setup
// Topic id : part4/section03/clang_format_setup
//
// Covers: .clang-format style gate

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // BasedOnStyle: LLVM / Google; IndentWidth: 4
    std::string style = "LLVM";
    assert(!style.empty());
}

void demo_intermediate() {
    // clang-format -i file.cpp; pre-commit hook
    assert(true);
}

void demo_expert() {
    // // clang-format off/on sparingly
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section03/clang_format_setup", run>;

}  // namespace
