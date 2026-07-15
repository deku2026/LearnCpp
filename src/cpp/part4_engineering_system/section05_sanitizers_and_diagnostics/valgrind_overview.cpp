// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : valgrind_overview
// Topic id : part4/section05/valgrind_overview
//
// Covers: Valgrind memcheck (Linux)

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // valgrind --leak-check=full ./app
    std::string tool = "valgrind";
    LEARN_CHECK(tool == "valgrind");
}

void demo_intermediate() {
    // Slower than ASan; different bug classes
    LEARN_CHECK(true);
}

void demo_expert() {
    // Not available natively on Windows
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/valgrind_overview", run>;

}  // namespace
