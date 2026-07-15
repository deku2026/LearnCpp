// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : ctest_integration
// Topic id : part4/section04/ctest_integration
//
// Covers: CTest add_test / ctest -R

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // enable_testing(); add_test(NAME t COMMAND app)
    std::string cmd = "ctest";
    LEARN_CHECK(cmd == "ctest");
}

void demo_intermediate() {
    // ctest --output-on-failure -j
    LEARN_CHECK(true);
}

void demo_expert() {
    // Labels and regex filters: ctest -R unit
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/ctest_integration", run>;

}  // namespace
