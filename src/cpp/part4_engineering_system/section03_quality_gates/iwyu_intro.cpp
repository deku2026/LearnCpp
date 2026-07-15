// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : iwyu_intro
// Topic id : part4/section03/iwyu_intro
//
// Covers: include-what-you-use

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // IWYU suggests minimal correct includes
    std::string header = "<vector>";
    assert(header.find("vector") != std::string::npos);
}

void demo_intermediate() {
    // Reduces rebuild surface and hidden dependencies
    assert(true);
}

void demo_expert() {
    // Forward declare in headers when possible
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section03/iwyu_intro", run>;

}  // namespace
